#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/wait.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <ctype.h>
#include <pcre.h>
#include "response.h"

#define PCRE_STATIC
#define MAX_IP_AMOUNT 10
#define OVECCOUNT 30

void error_die(const char *e)
{
    perror(e);
    exit(1);
}

in_addr_t *getIPs(int *IP_count)
{
  struct ifaddrs *ipaddr=NULL;
  in_addr_t IP,*IPs;
  int i=0;

    IPs=(in_addr_t *)malloc(sizeof(in_addr_t)*MAX_IP_AMOUNT);
    getifaddrs(&ipaddr);

    while(ipaddr!=NULL)
    {
        if(ipaddr->ifa_addr->sa_family==AF_INET)
        {
            IP=(((struct sockaddr_in *)ipaddr->ifa_addr)->sin_addr).s_addr;
            IPs[i++]=IP;
        }
        ipaddr=ipaddr->ifa_next;
    }

    *IP_count=i;

  return IPs;
}

int startup(in_addr_t currip,int port)
{
  int server_sock=-1;
  struct sockaddr_in server_addr;
  socklen_t server_addrlen=sizeof(server_addr);
  struct in_addr _in={currip};

    if((server_sock=socket(PF_INET,SOCK_STREAM,0))==-1)
        error_die("server_socket");

    memset(&server_addr,0,server_addrlen);
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(port);
    server_addr.sin_addr.s_addr=currip;

    if((bind(server_sock,(struct sockaddr *)&server_addr,server_addrlen))<0)
        error_die("bind");

    if (listen(server_sock,30)<0)
        error_die("listen");

    printf("%s:%d.\n",inet_ntoa(_in),port);

  return server_sock;
}

void write_log(int client,char *t,char *method,char *uri,char *query_str,int status)
{
  FILE *p;
  char path[1024],ip[20],buf[2048];
  struct sockaddr_in addr;
  socklen_t len=sizeof(addr);
    
    getcwd(path,sizeof(path));
    sprintf(path,"%s/access_log",path);
    getpeername(client,(struct sockaddr *)&addr,&len);
    inet_ntop(AF_INET,&addr.sin_addr,ip,sizeof(ip));
    if(strcmp(method,"GET")==0 && strlen(query_str)>0)
        sprintf(buf,"%s [%s +0800] \"%s %s?%s\" %d\n",ip,t,method,uri,query_str,status);
    else
        sprintf(buf,"%s [%s +0800] \"%s %s\" %d\n",ip,t,method,uri,status);

    p=fopen(path,"ab+");
    fwrite(buf,sizeof(char),strlen(buf),p);
    fclose(p);

  return;
}

int exec_cgi(int client,char *path,char *query_str)
{
  time_t t=time(0);
  char lt[255];
    
    strftime(lt,255,"%d/%b/%Y:%H:%M:%S",localtime(&t));
    response_500(client,lt);
    return 500;
}

int response(int client,char *method,char *uri,char *query_str)
{
  char path[1024],type[16];
  char buf,bt[255];
  struct stat st;
  FILE *pfile;
  time_t t=time(0);
  int cgi=0;

    strftime(bt,255,"%a, %d %b %Y %H:%M:%S",gmtime(&t));

    if(strcasecmp(method,"GET")==0){
        query_str=strchr(uri,'?');
        if(query_str){
            cgi=1;
            query_str++;
            uri=strtok(uri,"?");
        }
    }
    else
        if(strcasecmp(method,"POST")==0)
            if(strlen(query_str)==0){
                response_400(client,bt);
                return 400;
            }
        else
            cgi=1;
    else{
        response_501(client,method,bt);
        return 501;
    }

    if(strcmp(uri,"/")==0)
        strcat(uri,"index.html");
    
    getcwd(path,sizeof(path));
    sprintf(path,"%s/htdocs%s",path,uri);

    lstat(path,&st);
    if(S_ISDIR(st.st_mode)){
        response_403(client,uri,bt);
        return 403;
    }

    pfile=fopen(path,"rb+");
    if(pfile==NULL){
        response_404(client,uri,bt);
        return 404;
    }

        if(strstr(uri,".jpg")!=NULL)
            strcpy(type,"image/jpeg");
    else
        if(strstr(uri,".png")!=NULL)
            strcpy(type,"image/png");
    else
        if(strstr(uri,".ico")!=NULL)
            strcpy(type,"image/x-icon");
    else
        if(strstr(uri,".css")!=NULL)
            strcpy(type,"text/css");
    else
        if(strstr(uri,".js")!=NULL)
            strcpy(type,"application/javascript");
    else
        strcpy(type,"text/html");

    if(cgi==0){
        response_200(client,type,bt);
        fread(&buf,1,1,pfile);
        while(!feof(pfile))
        {
            send(client,&buf,1,0);
            fread(&buf,1,1,pfile);
        }
        fclose(pfile);
        close(client);
        return 200;
    }
    else
        return exec_cgi(client,path,query_str);
}

void *analyze_request(int *client_sock)
{
  int offset,rc,client=*client_sock;
  int ovector[OVECCOUNT];
  const char *error;
  char pattern[]="(GET|POST|HEAD|PUT|DELETE|CONNECT|OPTIONS|TRACE) (/([a-zA-Z0-9&%_./-~-]*)?).*\r\n\r\n(.*)";
  pcre *re=pcre_compile(pattern,PCRE_DOTALL,&error,&offset,NULL);

  int i=0,status;
  char *j;
  time_t t=time(0);
  char buf[1024],bt[255],lt[255];
  char method[10],uri[128],url[128],query_str[1024];
  strftime(bt,255,"%a, %d %b %Y %H:%M:%S",gmtime(&t));
  strftime(lt,255,"%d/%b/%Y:%H:%M:%S",localtime(&t));

    if(re==NULL) error_die("PCRE");

    recv(client,&buf,1024,0);
    buf[1023]='\0';
    printf("%s\n",buf);

    rc=pcre_exec(re,NULL,buf,1023,0,0,ovector,OVECCOUNT);
    if(rc<0){
        write_log(client,lt,method,uri,query_str,400);
        response_400(client,bt);
        return (void *)0;
    }
    else
    {
        for(j=buf+ovector[2];j<buf+ovector[3];j++)
            method[i++]=*j;
        i=0;
        for(j=buf+ovector[4];j<buf+ovector[5];j++)
            uri[i++]=*j;
        i=0;
        for(j=buf+ovector[8];j<buf+ovector[9];j++)
            query_str[i++]=*j;
    }

    strcpy(url,uri);
    status=response(client,method,uri,query_str);
    write_log(client,lt,method,url,query_str,status);

  return (void *)0;
}

int main(int argc,char *argv[])
{
  int server_sock=-1,client_sock=-1;
  int IP_count=0,i,pid;
  in_addr_t *IP;
  struct sockaddr_in client_addr;
  socklen_t client_addrlen=sizeof(client_addr);
  pthread_t thread;

    if(argc==1){
        printf("Parameter Error\n");
        return 1;
    }

    if(strcasecmp(argv[1],"-a")==0)
        IP=getIPs(&IP_count);
    else{
        IP_count=1;
        IP=(in_addr_t *)malloc(sizeof(in_addr_t));
        *IP=inet_addr(argv[1]);
    }

    printf("Http server is running on\n");
    
    for(i=0;i<IP_count;i++)
    {
        pid=fork();
        if(pid<0) error_die("fork");
        if(pid==0) continue;

        if(pid>0){
            if(argc==2)
                server_sock=startup(IP[i],8080);
            else
                server_sock=startup(IP[i],atoi(argv[2]));

            while(1){
                if((client_sock=accept(server_sock,(struct sockaddr *)&client_addr,&client_addrlen))==-1)
                    error_die("accept");
                if(pthread_create(&thread,NULL,(void *)analyze_request,&client_sock))
                    perror("pthread_create");
            }
            close(server_sock);     
        }
    }
    
  return 0;
}
