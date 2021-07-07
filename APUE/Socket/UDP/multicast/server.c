#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <net/if.h>
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

