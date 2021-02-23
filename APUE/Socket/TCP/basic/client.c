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
    

    raddr.sin_family = AF_INET;
    raddr.sin_port = htons(atoi(SERVERPORT));
    inet_pton(AF_INET,"127.0.0.1",&raddr.sin_addr);

    if(connect(sfd,(void *)&raddr,sizeof(raddr)) < 0){
        perror("connect()");
        exit(1);
    }

    FILE *fp;
    fp = fdopen(sfd,"r+");
    if (fp == NULL){
        perror("fopen()");
        exit(1);
    }
    

    puts("OK");

    close(sfd);

    exit(0);
}
