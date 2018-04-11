#include "types.h"
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>	
/*********************************************************************************
   *Function: CreateMsg
   * Description：创建消息
   *Input:  消息结构体，消息内容，消息长度，消息类型
   *Return:  NULL
   *Others:  NULL
 **********************************************************************************/ 
void CreateMsg(struct TcpMsg *m, char *str, size_t len,unsigned char type) 
{
    m->version = 1;
    m->type = type;
    m->len = len;
    m->value = malloc(len);
    memcpy(m->value, str, len);
}
/*********************************************************************************
   *Function: DeleteMsg
   * Description：删除消息申请的空间
   *Input:  消息结构体
   *Return:  NULL
   *Others:  NULL
 **********************************************************************************/
void DeleteMsg(struct TcpMsg *m) 
{
    if (m->value) {
        free(m->value);
    }
}
/*********************************************************************************
   *Function: MsgToData
   * Description：消息结构体转为unsigned char类型数据
   *Input:  消息结构体，返回数据内容，返回数据长度
   *Return:  NULL
   *Others:  NULL
 **********************************************************************************/
int MsgToData(struct TcpMsg *m, unsigned char **data, size_t *len) 
{
	if(m->version==1){//版本1
		size_t l = sizeof(struct TcpMsg) - sizeof(unsigned char *); // 除去unsigned char 的大小
		*len = l;
		*len += m->len;
		unsigned char * buf = malloc(*len);
		buf[0] = m->version;
		buf[1] = m->type;
		*(unsigned short *)(buf+2) = htons(m->len);
		memcpy(buf + 4, m->value, m->len);
		*data = buf;
		return 0;
	}
	else {
		printf("version incompatible\n");
		return -1;
	}
	
}
/*********************************************************************************
   *Function: DataToMsg
   * Description：unsigned char类型数据转为消息结构体
   *Input:  数据内容，数据长度，消息结构体
   *Return:  NULL
   *Others:  NULL
 **********************************************************************************/
int DataToMsg(unsigned char *data, size_t len, struct TcpMsg *m) 
{
    size_t l = sizeof(struct TcpMsg) - sizeof(unsigned char *); // 除去unsigned char 的大小
    
	if(data[0]==1){//版本1
		m->version = data[0];
		m->type = data[1];
		unsigned short t = *(unsigned short*)(data+2);
		m->len = ntohs(t);
		m->value = malloc(m->len);
		memcpy(m->value, data+l, m->len);
		return 0;
	}
	else {
		printf("version incompatible\n");
		return -1;
	}
}
/*********************************************************************************
   *Function: PrintfMsg
   * Description：打印消息
   *Input:  消息结构体
   *Return:  NULL
   *Others:  NULL
 **********************************************************************************/
void PrintfMsg(struct TcpMsg *m)
{
	printf("version=%d\n",m->version);
	
	switch((int)m->type)
	{
		case 1:
			printf("type=Client Send to Server\n");
		break;
		case 2:
			printf("type=Server Send to Client\n");
		break;
		case 3:
			printf("type=Client Send Wrong to Server\n");
		break;
		case 4:
			printf("type=Server Send Wrong to Client\n");
		break;
		case 5:
			printf("type=ACK\n");
		break;
	}
	
	printf("len=%d\n",m->len);
	printf("value=%s\n",m->value);
	printf("\n\n");
}
/*********************************************************************************
   *Function: SendAck
   * Description：发送确认
   *Input:  消息类型，套接字，消息内容，消息大小
   *Return:  NULL
   *Others:  NULL
 **********************************************************************************/
void SendAck(unsigned char type,int sockfd,char buf[],int bufsize)
{
	struct TcpMsg *sbuf;
	unsigned char *data = NULL;
	size_t len = 0;
	int writebytes;
	int ret;
	CreateMsg(&sbuf,buf, bufsize,type);
	
	ret=MsgToData(&sbuf, &data, &len);
	if(ret==0)DeleteMsg(&sbuf);//释放空间
	
	if((writebytes=send(sockfd,data,MAXSIZE,0))==-1){
		perror("send");
		close(sockfd);
		exit(1);
	}
	
	
}
/*********************************************************************************
   *Function: ClientRecvMsg
   * Description：客户端收到消息的处理
   *Input:  套接字，消息内容，消息大小
   *Return:  收到数据的长度
   *Others:  NULL
 **********************************************************************************/
int ClientRecvMsg(int sockfd,char buf[],int bufsize)
{
	int readbytes;
	struct TcpMsg rbuf;
	int ret;
	if((readbytes=recv(sockfd,buf,bufsize,0))==-1){
		perror("recv");
		close(sockfd);
		exit(1);
	}
	
	ret=DataToMsg(buf, readbytes, &rbuf);
	DealMsg(sockfd,&rbuf,false);
	if(ret==0)DeleteMsg(&rbuf);//释放空间
	
	return readbytes;
}
/*********************************************************************************
   *Function: ServerRecvMsg
   * Description：服务器端收到消息的处理
   *Input:  套接字，消息内容，消息大小
   *Return:  收到数据的长度，-2：客户端请求退出,-1:接收超时
   *Others:  NULL
 **********************************************************************************/
int ServerRecvMsg(int sockfd,char buf[],int bufsize)
{
	int readbytes;
	struct TcpMsg rbuf;
	int ret;
	if((readbytes=recv(sockfd,buf,bufsize,0))==-1){
		perror("recv");
		close(sockfd);
		//exit(1);
		return -1;	
	}
	
	ret=DataToMsg(buf, readbytes, &rbuf);	
	
	if(DealMsg(sockfd,&rbuf,true)==-1){
		return -2;
	}
	
	if(ret==0)DeleteMsg(&rbuf);//释放空间
	return readbytes;
}
/*********************************************************************************
   *Function: SendMsg
   * Description：发送消息
   *Input:  套接字，消息内容，消息大小
   *Return:  发送数据的长度
   *Others:  NULL
 **********************************************************************************/
int SendMsg(int sockfd,char buf[],int bufsize,unsigned char type)
{
	int writebytes;
	struct TcpMsg sbuf;
	unsigned char *data = NULL;
	size_t len = 0;
	int ret;
	CreateMsg(&sbuf,buf, strlen(buf),type);
	ret=MsgToData(&sbuf, &data, &len); 
	if(ret==0)DeleteMsg(&sbuf);//释放空间
	
	if((writebytes=send(sockfd,data,MAXSIZE,0))==-1){
		perror("send");
		close(sockfd);
		exit(1);
	}
	printf("send success\n");
	
	return writebytes;
}
/*********************************************************************************
   *Function: DealMsg
   * Description：消息处理，类型和长度处理
   *Input:  套接字，消息结构体，是否为服务器端
   *Return:  -1：客户端请求退出，0：收到错误数据，返回消息类型
   *Others:  NULL
 **********************************************************************************/
int DealMsg(int sockfd,struct TcpMsg *rbuf,bool isServer)
{
	if(rbuf->len==0){//空值
		printf("buf is null\n");
		
		if(isServer)//服务器端收到空值
			SendAck(CtoSWrong,sockfd,"Null",strlen("Null"));		
		else//客户端收到空值
			SendAck(StoCWrong,sockfd,"Null",strlen("Null"));			
	}
	if(rbuf->len>65535){//超出unsigned short边界值
		printf("buf is override\n");
		
		if(isServer)//服务器端收到溢出值
			SendAck(CtoSWrong,sockfd,"Override",strlen("Override"));
		else//客户端收到溢出值
			SendAck(StoCWrong,sockfd,"Override",strlen("Override"));
	}
	else{
		switch((int)rbuf->type)
		{
			case CtoS:
				if(strcmp(rbuf->value,"quit")==0){//当客户端发来的信息为 quit，则退出线程，并删除该结点
					return -1;
				}
				PrintfMsg(rbuf);
				SendAck(ACK,sockfd,rbuf->value,rbuf->len);
				return 1;
			break;
			
			case StoC:
				PrintfMsg(rbuf);
				return 2;
			break;
			
			case CtoSWrong:
				PrintfMsg(rbuf);
				return 3;
			break;
			
			case StoCWrong:
				PrintfMsg(rbuf);
				return 3;
			break;
			
			case ACK:
				PrintfMsg(rbuf);
				return 5;
			break;
			default:
				printf("recv invaild message type\n");
			break;
		}
	}
	return 0;
}