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

#define BUFSIZE 1024

int main()
{
    int sfd;
    struct sockaddr_in raddr;//remote addr

    sfd = socket(AF_INET,SOCK_STREAM,0/*IPPROTO_TCP*/);

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

    long long stamp;
    if (fscanf(fp,FMT_STAMP,&stamp) < 1){
        fprintf(stderr,"Bad format\n");
    }else{
        fprintf(stdout,FMT_STAMP,stamp);
    }

    close(sfd);

    exit(0);
}
