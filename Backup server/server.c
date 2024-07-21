#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 1024
void error(const char *msg){
perror(msg);
exit(1);
}
int main(int argc,char *argv[]){
if(argc<2){
fprintf(stderr,"Usage:%s port\n",argv[0]);
}
int sockfd,newsockfd,portno;
socklen_t clilen;
char buffer[BUFFER_SIZE];
struct sockaddr_in serv_addr,cli_addr;
int n;
sockfd =socket(AF_INET,SOCK_STREAM,0);
if(sockfd<0)
error ("error opening socket");
bzero((char *)&serv_addr,sizeof(serv_addr));
portno = atoi(argv[1]);
serv_addr.sin_family = AF_INET;
serv_addr.sin_addr.s_addr = INADDR_ANY;
serv_addr.sin_port = htons(portno);
if(bind(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
error("error on binding");
listen(sockfd,5);
clilen = sizeof(cli_addr);
newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr,&clilen);
if(newsockfd<0)
error("error on accept");
FILE *fp = fopen("backup.txt","w");
if(fp==NULL)
error("error opening file");
bzero(buffer,BUFFER_SIZE);
while((n = recv(newsockfd,buffer,BUFFER_SIZE,0))>0){
fwrite(buffer,sizeof(char),n,fp);
bzero(buffer,BUFFER_SIZE);
}
if(n<0)
error("error reading from socket");
fclose(fp);
close(newsockfd);
close(sockfd);
return 0;
}
