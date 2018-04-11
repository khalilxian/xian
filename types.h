#ifndef types_h
#define types_h
#include <sys/types.h>
#include <stdbool.h>
#include <sys/time.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>

#define CtoS 1
#define StoC 2
#define CtoSWrong 3
#define StoCWrong 4
#define ACK 5

#define LISMAX 100//监听最大值
#define MAXSIZE 2048//数组空间
#define PORT 8080//端口号

//一字节对齐
#pragma pack(1)
struct TcpMsg {
    unsigned char version;
    unsigned char type;
    unsigned short len;
    unsigned char *value;
};
#pragma pack()


void CreateMsg(struct TcpMsg *m, char *str, size_t len,unsigned char type);
void DeleteMsg(struct TcpMsg *m);
int MsgToData(struct TcpMsg *m, unsigned char **data, size_t *len);
int DataToMsg(unsigned char *data, size_t len, struct TcpMsg *m);
void PrintfMsg(struct TcpMsg *m);
void SendAck(unsigned char type,int sockfd,char buf[],int bufsize);
int ClientRecvMsg(int sockfd,char buf[],int bufsize);
int ServerRecvMsg(int sockfd,char buf[],int bufsize);
int SendMsg(int sockfd,char buf[],int bufsize,unsigned char type);
int DealMsg(int sockfd,struct TcpMsg *rbuf,bool isServer);
#endif /* types_h */
