# 跨主机IPC(进程间通信)

## 网络套接字(socket)

## 跨主机的传输要注意的问题
### 字节序
- 大端 **低地址放高字节**
- 小端 **高地址放低字节（x86）**
~~~ c
0x00 00 00 05

//大端
05 00 00 00
//小端
00 00 00 05
~~~
- 主机字节序 **host**
- 网络字节序 **network**
- _ to _ 长度()
    - htons()
    - htonl()
    - ntohs()
    - ntohl()
### 对齐
~~~ c
struct{
    int i;
    char ch;
    float f;
};//12个字节
~~~
结构体内存对齐
- 解决方案 **指定宏 告诉编译器不对齐**
### 类型长度
- int的长度未定义
- char有无符号未定义
解决： `int32_t` `uint32_t` `int64_t` `int8_t(有符号的char)` `uint8_t(无符号的char)`

### socket
**一个中间层，连接网络协议与文件操作**

~~~ c
int socket(int domain,int type,int protocol)
          //SOCK_STREAM 有序 可靠 双工 基于(单字节)字节流
          //SOCK_DGRAM 分组的 不可靠的 无连接
          //SOCK_SEQPACKET (有序分组式) 有序可靠 报式传输
~~~

#### 常用函数
绑定地址和端口
- int bind(int socket, const struct sockaddr \*address, socklen_t address_len)
sockaddr -> struct socketaddr_in
~~~ c
 struct sockaddr_in {
               sa_family_t    sin_family; /* address family: AF_INET */
               in_port_t      sin_port;   /* port in network byte order */
               struct in_addr sin_addr;   /* internet address */
           };

~~~

- recvfrom()
- sendto()

- inet_pton() 点分式转二进制数

## UDP
### 步骤
- 被动端
    - 取得SOCKET
    - 给SOCKET取得地址
    - 收/发消息
    - 关闭SOCKET
- 主动端
    - 取得SOCKET
    - 给SOCKET取得地址(可以省略 本地通信地址由操作系统分配)
    - 发/收消息
    - 关闭SOCKET

~~~ c
#ifndef PROTO_H__
#define PROTO_H__

#include <stdint.h>

#define NAMESIZE 128

struct msg_st{
    uint8_t name[NAMESIZE];
    uint32_t math;
    uint32_t chinese;
}__attribute__((packed));//不对齐

#endif

~~~

~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "proto.h"

#define IPSIZE 1024
#define SERVERPORT "2333"


int main()
{
    int sfd;
    struct sockaddr_in laddr;//local addr
    struct sockaddr_in raddr;//remote addr
    struct msg_st rbuf;
    char ip[IPSIZE];

    sfd = socket(AF_INET,SOCK_DGRAM,0/*IPPROTO_UDP*/);
    if (sfd < 0){
        perror("socket()");
        exit(1);
    }

    laddr.sin_family = AF_INET;//指定协议
    laddr.sin_port = htons(atoi(SERVERPORT));//指定网络通信端口
    inet_pton(AF_INET,"0.0.0.0",&laddr.sin_addr);//IPv4点分式转二进制数

    if(bind(sfd,(void *)&laddr,sizeof(laddr)) < 0){
        perror("bind()");
        exit(1);
    }

    socklen_t raddr_len = sizeof(raddr);
    while(1){
        recvfrom(sfd,&rbuf,sizeof(rbuf),0,(void *)&raddr,&raddr_len);//报式套接字每次通信都需要知道对方是谁
        inet_ntop(AF_INET,&raddr.sin_addr,ip,IPSIZE);
        printf("%s %d\n",ip,ntohs(raddr.sin_port));
        printf("%s %d %d\n",rbuf.name,ntohl(rbuf.math),ntohl(rbuf.chinese));
        fflush(NULL);
    }

    close(sfd);
    
    exit(0);
}

~~~

~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

#include "proto.h"

#define SERVERPORT "2333"

int main()
{
    int sfd;
    struct msg_st sbuf;
    struct sockaddr_in raddr;//remote addr

    sfd = socket(AF_INET,SOCK_DGRAM,0/*IPPROTO_UDP*/);
    
    strcpy(sbuf.name,"Alan");
    sbuf.math = htonl(99);//主机字节序转网络字节序
    sbuf.chinese = htonl(89);

    raddr.sin_family = AF_INET;
    raddr.sin_port = htons(atoi(SERVERPORT));
    inet_pton(AF_INET,"127.0.0.1",&raddr.sin_addr);

    if(sendto(sfd,&sbuf,sizeof(sbuf),0,(void *)&raddr,sizeof(raddr)) < 0){
        perror("sendto()");
        exit(1);
    }

    puts("OK");

    close(sfd);

    exit(0);
}
~~~


## TCP

