#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define BUFFER_SIZE 1024
void error(const char *msg){
perror(msg);
exit(1);
}
int main(int argc,char *argv[]){
if(argc<4){
fprintf(stderr,"usage:%s hostname port file\n",argv[0]);
exit(1);
}
int sockfd,portno,n;
struct sockaddr_in serv_addr;
struct hostent *server;
char buffer[BUFFER_SIZE];
portno = atoi(argv[2]);
sockfd = socket(AF_INET,SOCK_STREAM,0);
if(sockfd<0)
error("error opening socket");
server = gethostbyname(argv[1]);
if(server ==NULL){
fprintf(stderr,"error,no such host\n");
exit(1);
}
bzero((char *)&serv_addr,sizeof(serv_addr));
serv_addr.sin_family = AF_INET;
bcopy((char  *)server->h_addr,(char *)&serv_addr.sin_addr,server->h_length);
serv_addr.sin_port = htons(portno);
if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
error("error connecting");
FILE *fp = fopen(argv[3],"r");
if(fp==NULL)
error("errror opening file");
bzero(buffer,BUFFER_SIZE);
while((n = fread(buffer,sizeof(char),BUFFER_SIZE,fp))>0){
if(send(sockfd,buffer,n,0)<0)
error("error writing to socket");
bzero(buffer,BUFFER_SIZE);
}
fclose(fp);
close(sockfd);
return 0;
}
