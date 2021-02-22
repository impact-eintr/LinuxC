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
    int pkglen = sizeof(struct msg_st)+strlen("Mike");
    sbuf = malloc(pkglen);

    strcpy(sbuf->name,"Mike");
    sbuf->math = htonl(99);//主机字节序转网络字节序
    sbuf->chinese = htonl(89);

    raddr.sin_family = AF_INET;
    raddr.sin_port = htons(atoi(SERVERPORT));
    inet_pton(AF_INET,"127.0.0.1",&raddr.sin_addr);

    if(sendto(sfd,&sbuf,pkglen,0,(void *)&raddr,sizeof(raddr)) < 0){
        perror("sendto()");
        exit(1);
    }

    puts("OK");

    close(sfd);

    exit(0);
}
