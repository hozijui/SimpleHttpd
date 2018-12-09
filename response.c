#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "response.h"


#define SERVER "Server: zjui's WebServer\r\n"

void response_200(int client,char *type,char *t)
{
  char buf[1024];

    sprintf(buf,"HTTP/1.1 200 OK\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"Content-Type: %s\r\n",type);
    send(client,buf,strlen(buf),0);
    sprintf(buf,"Date: %s GMT\r\n",t);
    send(client,buf,strlen(buf),0);
    sprintf(buf,SERVER);
    send(client,buf,strlen(buf),0);
    sprintf(buf,"\r\n");
    send(client,buf,strlen(buf),0);

  return;
}

void response_400(int client,char *t)
{
  char buf[1024];
    sprintf(buf,"HTTP/1.1 400 Bad Reuqest\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"Connection: close\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"Content-Type: text/html\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"Date: %s GMT\r\n",t);
    send(client,buf,strlen(buf),0);
    sprintf(buf,SERVER);
    send(client,buf,strlen(buf),0);
    sprintf(buf,"\r\n");
    send(client,buf,strlen(buf),0);

    sprintf(buf,"<html><head>\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"<title>400 Bad Request</title>\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"</head><body>\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"<h1>Bad Request</h1>\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"</body></html>\r\n");
    send(client,buf,strlen(buf),0);
    close(client);

  return;
}
  
void response_403(int client,char *uri,char *t)
{
  char buf[1024];
    sprintf(buf,"HTTP/1.1 403 Forbidden\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"Connection: close\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"Content-Type: text/html\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"Date: %s GMT\r\n",t);
    send(client,buf,strlen(buf),0);
    sprintf(buf,SERVER);
    send(client,buf,strlen(buf),0);
    sprintf(buf,"\r\n");
    send(client,buf,strlen(buf),0);

    sprintf(buf,"<html><head>\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"<title>403 Forbidden</title>\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"</head><body>\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"<h1>Forbidden</h1>\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"<hr><p>You don't have permission to access %s on this server.</p>",uri);
    send(client,buf,strlen(buf),0);
    sprintf(buf,"</body></html>\r\n");
    send(client,buf,strlen(buf),0);
    close(client);

  return;
}
  
void response_404(int client,char *uri,char *t)
{
  char buf[1024];
    sprintf(buf,"HTTP/1.1 404 Not Found\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"Connection: close\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"Content-Type: text/html\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"Date: %s GMT\r\n",t);
    send(client,buf,strlen(buf),0);
    sprintf(buf,SERVER);
    send(client,buf,strlen(buf),0);
    sprintf(buf,"\r\n");
    send(client,buf,strlen(buf),0);

    sprintf(buf,"<html><head>\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"<title>404 Not Found</title>\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"</head><body>\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"<h1>Not Found</h1>\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"<hr><p>The requested URL %s was not found on this server.</p>",uri);
    send(client,buf,strlen(buf),0);
    sprintf(buf,"</body></html>\r\n");
    send(client,buf,strlen(buf),0);
    close(client);

  return;
}

void response_500(int client,char *t)
{
  char buf[1024];
    sprintf(buf,"HTTP/1.1 500 Internal Server Error\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"Connection: close\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"Content-Type: text/html\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"Date: %s GMT\r\n",t);
    send(client,buf,strlen(buf),0);
    sprintf(buf,SERVER);
    send(client,buf,strlen(buf),0);
    sprintf(buf,"\r\n");
    send(client,buf,strlen(buf),0);

    sprintf(buf,"<html><head>\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"<title>500 Internal Server Error</title>\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"</head><body>\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"<h1>Internal Server Error</h1>\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"</body></html>\r\n");
    send(client,buf,strlen(buf),0);
    close(client);

  return;
}

void response_501(int client,char *method,char *t)
{
  char buf[1024];
    sprintf(buf,"HTTP/1.1 501 Not Implemented\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"Connection: close\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"Content-Type: text/html\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"Date: %s GMT\r\n",t);
    send(client,buf,strlen(buf),0);
    sprintf(buf,SERVER);
    send(client,buf,strlen(buf),0);
    sprintf(buf,"\r\n");
    send(client,buf,strlen(buf),0);

    sprintf(buf,"<html><head>\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"<title>501 Not Implemented</title>\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"</head><body>\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"<h1>Not Implemented</h1>\r\n");
    send(client,buf,strlen(buf),0);
    sprintf(buf,"<hr><p>The requested method %s is not supported on this server.</p>",method);
    send(client,buf,strlen(buf),0);
    sprintf(buf,"</body></html>\r\n");
    send(client,buf,strlen(buf),0);
    close(client);

  return;
}