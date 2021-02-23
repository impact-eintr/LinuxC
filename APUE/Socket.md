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

- socket()
- bind()
- sendto()
- rcvfrom()
- inet_pton()
- inet_ntop()

~~~ c
#ifndef PROTO_H__
#define PROTO_H__

#include <stdint.h>

#define NAMEMAX 512-8-8//(UDP推荐长度-UDP报头长度-结构体的长度)

struct msg_st{
    uint32_t math;
    uint32_t chinese;
    char name[0];
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
#include <stdarg.h>

#include "proto.h"

#define IPSIZE 1024
#define SERVERPORT "2333"

static void debug(char *fmt,...){
    va_list ap;
    va_start(ap,fmt);

    printf("DEBUG ");
    printf(fmt,va_arg(ap,int));

    va_end(ap);
}

int main()
{
    int sfd;
    struct sockaddr_in laddr;//local addr
    struct sockaddr_in raddr;//remote addr
    struct msg_st *rbuf;
    char ip[IPSIZE];

    int pkglen = sizeof(struct msg_st)+NAMEMAX;
    debug("%d\n",pkglen);
    rbuf = malloc(pkglen);
    if (rbuf == NULL){
        perror("malloc()");
        exit(1);
    }

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
        recvfrom(sfd,rbuf,pkglen,0,(void *)&raddr,&raddr_len);//报式套接字每次通信都需要知道对方是谁
        inet_ntop(AF_INET,&raddr.sin_addr,ip,IPSIZE);
        printf("%s %d\n",ip,ntohs(raddr.sin_port));
        printf("%s %d %d\n",rbuf->name,ntohl(rbuf->math),ntohl(rbuf->chinese));
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
    struct msg_st *sbuf;
    struct sockaddr_in raddr;//remote addr

    sfd = socket(AF_INET,SOCK_DGRAM,0/*IPPROTO_UDP*/);
    int pkglen = sizeof(struct msg_st)+strlen("Mike")+1;// 注意给'/0'留位置
    sbuf = malloc(pkglen);
    if (sbuf == NULL){
        perror("malloc()");
        exit(1);
    }
    
    char *name = "Mike";
    strcpy(sbuf->name,name);
    sbuf->math = htonl(rand()%100);//主机字节序转网络字节序
    sbuf->chinese = htonl(rand()%100);

    raddr.sin_family = AF_INET;
    raddr.sin_port = htons(atoi(SERVERPORT));
    inet_pton(AF_INET,"127.0.0.1",&raddr.sin_addr);

    if(sendto(sfd,sbuf,pkglen,0,(void *)&raddr,sizeof(raddr)) < 0){
        perror("sendto()");
        exit(1);
    }

    puts("OK");

    close(sfd);

    exit(0);
}
~~~

### 多点通讯
#### 广播(全网广播，子网广播)
- getsockopt()
- setsockopt()

~~~ c
//client.c
int main()
{
    int sfd;
    struct msg_st *sbuf;
    struct sockaddr_in raddr;//remote addr

    sfd = socket(AF_INET,SOCK_DGRAM,0/*IPPROTO_UDP*/);
    if(sfd < 0){
        perror("socket()");
        exit(1);
    }
    //设置socket的属性
    int val = 1;
    if (setsockopt(sfd,SOL_SOCKET,SO_BROADCAST,&val,sizeof(val)) < 0){
        perror("setsockopt()");
        exit(1);
    }//打开广播属性
    

    int pkglen = sizeof(struct msg_st)+strlen("Mike")+1;// 注意给'/0'留位置
    sbuf = malloc(pkglen);
    if (sbuf == NULL){
        perror("malloc()");
        exit(1);
    }
    
    char *name = "Mike";
    strcpy(sbuf->name,name);
    sbuf->math = htonl(rand()%100);//主机字节序转网络字节序
    sbuf->chinese = htonl(rand()%100);

    raddr.sin_family = AF_INET;
    raddr.sin_port = htons(atoi(SERVERPORT));
    inet_pton(AF_INET,"255.255.255.255",&raddr.sin_addr);

    if(sendto(sfd,sbuf,pkglen,0,(void *)&raddr,sizeof(raddr)) < 0){
        perror("sendto()");
        exit(1);
    }

    puts("OK");

    close(sfd);

    exit(0);
}
~~~

~~~ c
//server.c
int main()
{
    int sfd;
    struct sockaddr_in laddr;//local addr
    struct sockaddr_in raddr;//remote addr
    struct msg_st *rbuf;
    char ip[IPSIZE];

    int pkglen = sizeof(struct msg_st)+NAMEMAX;
    debug("%d\n",pkglen);
    rbuf = malloc(pkglen);
    if (rbuf == NULL){
        perror("malloc()");
        exit(1);
    }

    sfd = socket(AF_INET,SOCK_DGRAM,0/*IPPROTO_UDP*/);
    if (sfd < 0){
        perror("socket()");
        exit(1);
    }

    //设置socket属性
    int val = 1;
    if(setsockopt(sfd,SOL_SOCKET,SO_BROADCAST,&val,sizeof(val)) < 0){
        perror("setsockopt()");
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
        recvfrom(sfd,rbuf,pkglen,0,(void *)&raddr,&raddr_len);//报式套接字每次通信都需要知道对方是谁
        inet_ntop(AF_INET,&raddr.sin_addr,ip,IPSIZE);
        printf("%s %d\n",ip,ntohs(raddr.sin_port));
        printf("%s %d %d\n",rbuf->name,ntohl(rbuf->math),ntohl(rbuf->chinese));
        fflush(NULL);
    }

    close(sfd);
    
    exit(0);
}

~~~

#### 多播/组播
相较广播更灵活
`224.0.0.1` **这个地址表示所有支持多播的节点默认都存在于这个组中且无法离开**

~~~ c
//client.c
int main()
{
    int sfd;
    struct msg_st *sbuf;
    struct sockaddr_in raddr;//remote addr
    struct ip_mreqn mreqn;

    sfd = socket(AF_INET,SOCK_DGRAM,0/*IPPROTO_UDP*/);
    if(sfd < 0){
        perror("socket()");
        exit(1);
    }
    //设置socket的属性
    inet_pton(AF_INET,"0.0.0.0",&mreqn.imr_address);
    inet_pton(AF_INET,MULTICASTADDR,&mreqn.imr_multiaddr);
    mreqn.imr_ifindex = if_nametoindex("wlp7s0");

    if (setsockopt(sfd,IPPROTO_IP,IP_MULTICAST_IF,&mreqn,sizeof(mreqn)) < 0){
        perror("setsockopt()");
        exit(1);
    }//打开广播属性
    

    int pkglen = sizeof(struct msg_st)+strlen("Mike")+1;// 注意给'/0'留位置
    sbuf = malloc(pkglen);
    if (sbuf == NULL){
        perror("malloc()");
        exit(1);
    }
    
    char *name = "Mike";
    strcpy(sbuf->name,name);
    sbuf->math = htonl(rand()%100);//主机字节序转网络字节序
    sbuf->chinese = htonl(rand()%100);

    raddr.sin_family = AF_INET;
    raddr.sin_port = htons(atoi(SERVERPORT));
    inet_pton(AF_INET,MULTICASTADDR,&raddr.sin_addr);

    if(sendto(sfd,sbuf,pkglen,0,(void *)&raddr,sizeof(raddr)) < 0){
        perror("sendto()");
        exit(1);
    }

    puts("OK");

    close(sfd);

    exit(0);
}
~~~

~~~ c
//server.c
int main()
{
    int sfd;
    struct sockaddr_in laddr;//local addr
    struct sockaddr_in raddr;//remote addr
    struct msg_st *rbuf;
    char ip[IPSIZE];

    int pkglen = sizeof(struct msg_st)+NAMEMAX;
    rbuf = malloc(pkglen);
    if (rbuf == NULL){
        perror("malloc()");
        exit(1);
    }

    sfd = socket(AF_INET,SOCK_DGRAM,0/*IPPROTO_UDP*/);
    if (sfd < 0){
        perror("socket()");
        exit(1);
    }

    //设置socket属性
    struct ip_mreqn mreqn;
    inet_pton(AF_INET,"0.0.0.0",&mreqn.imr_address);
    //224.0.0.1 这个地址表示所有支持多播的节点默认都存在于这个组中且无法离开
    inet_pton(AF_INET,MULTICASTADDR,&mreqn.imr_multiaddr);
    mreqn.imr_ifindex = if_nametoindex("wlp7s0");
    if (setsockopt(sfd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreqn,sizeof(mreqn)) < 0){
        perror("setsockopt()");
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
        recvfrom(sfd,rbuf,pkglen,0,(void *)&raddr,&raddr_len);//报式套接字每次通信都需要知道对方是谁
        inet_ntop(AF_INET,&raddr.sin_addr,ip,IPSIZE);
        printf("%s %d\n",ip,ntohs(raddr.sin_port));
        printf("%s %d %d\n",rbuf->name,ntohl(rbuf->math),ntohl(rbuf->chinese));
        fflush(NULL);
    }

    close(sfd);
    
    exit(0);
}

~~~

#### UDP需要特别注意的问题
**丢包是由阻塞造成的(网络被路由器或其他网络节点按照某种算法移除),而不是ttl**
- TTL time to live  数据包跳转的路由数


比如我们要实现一个ftp文件服务器的程序.

~~~ c
#ifndef PROTO_H__
#define PROTO_H__

#define PATHSIZE 1024
#define DATASIZE 1024

enum{
    MSG_PATH = 1,
    MSG_DATA,
    MSG_EOT//end_of_transfer
};

typedef struct msg_path_st{
    long mtype;
    char path[PATHSIZE];
} msg_path_t;


typedef struct msg_data_st{
    long mtype;
    char data[DATASIZE];
    int datalen;
} msg_data_t;

typedef struct msg_eot_st{
    long mtype;
} msg_eot_t;

union msg_s2c_un{
    long mtype;
    msg_data_t datamsg;
    msg_eot_t eofmsg;
};

#endif

~~~

Client&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Server
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;------文件路径--->
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;文件
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<-----data1--------
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<-----data2--------    
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<-----data3--------    
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<------EOT--------- 
文件


由于网络的不可靠，如果出现丢包将会使得文件传输不完整，如何确保对方一定能收到完整内容呢？
**停等式流控**

Client&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Server
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;--文件路径-->
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;文件
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<-----data1--------等待
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-------ACK1------>继续发送
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<-----data2--------等待    
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-------ACK2------>继续发送
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<-----data3--------等待    
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-------ACK3------>继续发送
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<------EOT---------等待    
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;--------ACK4----->停止发送
文件

中途如果有ACK包Server没有成功收到那么将会再次发送数据包，成功的两次传输耗时将成为界定是否超时的标准(RTT)



## TCP
### 从UDP到TCP


