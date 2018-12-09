#ifndef _RESPONSE_H_
#define _RESPONSE_H_

void response_200(int client,char *type,char *t);
void response_400(int client,char *t);
void response_403(int client,char *uri,char *t);
void response_404(int client,char *uri,char *t);
void response_500(int client,char *t);
void response_501(int client,char *method,char *t);

#endif