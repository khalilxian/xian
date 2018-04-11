#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <errno.h>  
#include <arpa/inet.h>  
#include <netdb.h>
#include <netinet/in.h>
#include "types.h"
#include <pthread.h>
void* thread(void* arg)
{
	int readbytes;
	char buf[MAXSIZE]={0};
    int sockfd = *((int*)arg);
	while(1)                                     //循环接收服务器端发来的消息
    {	
		//接收消息
		ClientRecvMsg(sockfd,buf,MAXSIZE);
		bzero(buf,MAXSIZE);
	}		
}
int main(int argc,char *argv[])
{
    int sockfd;
    int size,writebytes,readbytes;
    char buf[MAXSIZE]={0};
    pthread_t tid;
    struct sockaddr_in saddr;
	struct hostent *host;
	struct TcpMsg sbuf;
	unsigned char *data = NULL;
	size_t len = 0;
	
	if(argc < 2){
		fprintf(stderr,"Please enter the server's hostname!\n");
		exit(1);
	}
	if((host=gethostbyname(argv[1]))==NULL){
		perror("gethostbyname");
		exit(1);
	}
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1){
		perror("socket");
		exit(1);
	}
	size = sizeof(saddr);
    bzero(&saddr,size);
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(PORT);
    saddr.sin_addr=*((struct in_addr *)host->h_addr); 
	bzero(&(saddr.sin_zero),8);
	
	if(connect(sockfd,(struct sockaddr*)&saddr,size)==-1){
		perror("connect");
		close(sockfd);
		exit(1);
	}
	
	
	printf("connect success\n");
	ClientRecvMsg(sockfd,buf,MAXSIZE);
	
	while(1)//主线程用于给服务端发信息。
    {
		printf("please enter msg:");
        gets(buf);//危险函数
		if(strcmp(buf,"quit")==0){
			printf("request quit !\n");
			pthread_exit(NULL);
			close(sockfd);
			exit(1);
		}
		
		printf("buf len=%d\n\n",strlen(buf));
		
		//发消息
		SendMsg(sockfd,buf,MAXSIZE,CtoS);
		bzero(buf,MAXSIZE);
		
		//创建一个线程接收服务器端的消息
		pthread_create(&tid,NULL,thread,(void*)&sockfd); 
		
		
		
	}
	close(sockfd);
    return 0;
}
