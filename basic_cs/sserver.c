#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <errno.h>  
#include <arpa/inet.h>  
#include <pthread.h>
#include "types.h"

#define IPSIZE 20


struct client *head = NULL;
struct sockaddr_in saddr;
struct sockaddr_in caddr;
 
struct client
{
    int sock;
    struct client *next;
};
 
struct client *InitList()//创建客户端队列头
{
    struct client *head = malloc(sizeof(struct client));
    head->next = NULL;
    return head;
}
 
int AddNode(struct client*head,int new_sockfd)//创建客户端节点
{
    struct client *new = malloc(sizeof(struct client));
    struct client *p = head;
 
    new->sock = new_sockfd;
    new->next = NULL;
 
    while(p->next!=NULL)
    {
        p = p->next;
    }
    p->next = new;
    new->next = NULL;
    return 0;
}
 
int DelNode(struct client*head,int sockfd)//删除客户端节点
{
    struct client *q = head;
    struct client *p = q->next;
 
    while(p->sock !=sockfd)
    {
        if(p->next !=NULL)
        {
            p = p->next;
            q = q->next;
        }
        else if(p->next == NULL)//没有下一个socket
        {
            printf("can not find sockfd to del\n");
            return -1;
        }
    }
    q->next = p->next;
    free(p);
    return 0;
}
 
void* thread(void* arg)
{
    pthread_detach(pthread_self());              //设置该线程为分离状态，不需要主线程回收其系统资源。
    char buf[MAXSIZE];
    char ipbuf[IPSIZE];
    bzero(ipbuf,IPSIZE);
    bzero(buf,MAXSIZE);
    int port,readbytes;
    int sockfd = *((int*)arg);
	struct TcpMsg rbuf;
	int ret;
    while(1)                                     //循环接收客户端发来的信息
    {
		inet_ntop(AF_INET,(void*)&caddr.sin_addr.s_addr,ipbuf,IPSIZE);  //把客户端的ip信息放到ipbuf中
		port = ntohs(caddr.sin_port);                                   //把客户端的端口号放到变量port中
		
		
		ret=ServerRecvMsg(sockfd,&buf,MAXSIZE);
		printf("ret=%d\n",ret);
		
		if(ret==0){
			printf("ip:%s,port:%hu disconnect!\n",ipbuf,port);
			close(sockfd);
			pthread_exit(NULL);
		}

		if(ret==-2){            //当客户端发来的信息为 quit，则退出线程，并删除该结点
			printf("ip:%s,port:%hu disconnect!\n",ipbuf,port);
			DelNode(head,sockfd);
			close(sockfd);
			pthread_exit(NULL);
		}   	
		
		bzero(buf,MAXSIZE);
		memset(&rbuf,0,sizeof(rbuf));//清空结构体 
		 
    }
}
 
int main()
{
    int sockfd,clientfd,new_sockfd;
    int size,writebytes;
    pthread_t tid;
	struct TcpMsg sbuf;
	unsigned char *data = NULL;
	size_t len = 0;
		
    size = sizeof(struct sockaddr_in);
    head = InitList();
	
    /*打开一个网络通讯端口，分配一个文件描述符*/
	if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1){
		perror("socket");
		exit(1);
	}
	printf("socket success!,sockfd=%d\n",sockfd);
	
	bzero(&saddr,size);
    saddr.sin_family = AF_INET;                 //绑定协议域为 IPv4
    saddr.sin_port = htons(PORT);               //绑定端口号
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);  //绑定ip地址为本机的一个随机IP
	bzero(&(saddr.sin_zero),8);//清空服务端套接字
	
   /*将文件描述符和服务器地址绑定在一起*/
	if(bind(sockfd,(struct sockaddr*)&saddr,size)==-1){
		perror("bind");
		close(sockfd);
		exit(1);
	}
	printf("bind success!\n");
	
	
    //socket套接字和sockaddr_in结构体绑定在一起，赋予socket属性
    /*声明listenfd处于监听状态，并且最多允许有BACKLOG个客户端处于待连接状态*/
	if(listen(sockfd,LISMAX)==-1){
		perror("listen");
		close(sockfd);
		exit(1);
	}
	printf("listening....\n");                            //开始监听有没有客户端连接
	
	
    while(1)//循环监听有没有客户端连接进来，就像一个接待人员等待客人，有客人来了就服务他
    {
		/*服务器调用accept()接受连接*/
        if((new_sockfd = accept(sockfd,(struct sockaddr*)&caddr,&size))==-1){      //等待客户端连接，并返回该客户端的描述符
			perror("accept");
			close(new_sockfd);
			exit(1);
		}
		printf("accept success\n");
		
		SendMsg(new_sockfd,"accept success",strlen("accept success"),StoC);
		
        memset(&sbuf,0,sizeof(sbuf));//清空结构体
		
		AddNode(head,new_sockfd);                                       //若有客户端连接，则把该客户端加入到客户端队列中
        pthread_create(&tid,NULL,thread,(void*)&new_sockfd);            //创建一个线程服务新连接进来的客户端
 
    }
    pthread_exit(NULL);
	close(sockfd);
    return 0;
}
