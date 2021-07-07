#include <asm-generic/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <net/if.h>
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
