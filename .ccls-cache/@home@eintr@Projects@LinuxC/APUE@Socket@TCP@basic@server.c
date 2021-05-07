#include <asm-generic/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>

#include "proto.h"

#define IPSIZE 1024
#define BUFSIZE 1024
#define SERVERPORT "2333"

static void debug(char *fmt,...){
    va_list ap;
    va_start(ap,fmt);

    printf("DEBUG ");
    printf(fmt,va_arg(ap,int));

    va_end(ap);
}

static void server_job(int newsd){
    char buf[BUFSIZE];
    int pkglen = 0;

    pkglen = sprintf(buf,FMT_STAMP,(long long)time(NULL));

    if (send(newsd,buf,pkglen,0) < 0){
        perror("send()");
        exit(1);
    }
}

int main()
{
    int sfd;
    struct sockaddr_in laddr;//local addr
    struct sockaddr_in raddr;//remote addr
    char ip[IPSIZE];

    sfd = socket(AF_INET,SOCK_STREAM,0/*IPPROTO_TCP*/);
    if (sfd < 0){
        perror("socket()");
        exit(1);
    }
    
    int val = 1;
    if(setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val)) < 0){
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

    if(listen(sfd,1024) < 0){//全连接数量
        perror("listen()");
        exit(1);
    }


    socklen_t raddr_len = sizeof(raddr);
    while(1){
        int newsd;
        newsd = accept(sfd,(void *)&raddr,&raddr_len);//接收客户端连接
        if (newsd < 0){
            perror("accept()");
            exit(1);
        }
        
        inet_ntop(AF_INET,&raddr.sin_addr,ip,IPSIZE);
        printf("client %s %d\n",ip,ntohs(raddr.sin_port));
        server_job(newsd);
        close(newsd);
    }

    close(sfd);
    
    exit(0);
}

