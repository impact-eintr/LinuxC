#include "proto.h"
#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#define IPSTRSIZE 40

static void printmsg(struct msg_st* msg)
{
    printf("MANE = %s\n", msg->name);
    printf("MATH = %d\n", ntohl(msg->math));
    printf("CHINESE = %d\n", ntohl(msg->chinese));
}

int main()
{
    int sd;
    struct sockaddr_in laddr, raddr; //l local r remote遥远的
    char ipstr[IPSTRSIZE];
    struct msg_st rbuf;
    socklen_t raddr_len;

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0) {
        perror("socket()");
        exit(1);
    }

    struct ip_mreqn mreq;
    inet_pton(AF_INET, MTGROUP, &mreq.imr_multiaddr);
    inet_pton(AF_INET, "0.0.0.0", &mreq.imr_address);
    mreq.imr_ifindex = if_nametoindex("enp0s20f0u4u3u1");

    if (setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
        perror("socketopt()");
        exit(1);
    }

    laddr.sin_family = AF_INET;
    laddr.sin_port = htons(atoi(RCVPORT));
    inet_pton(AF_INET, "0.0.0.0", &laddr.sin_addr);

    if (bind(sd, (void*)&laddr, sizeof(laddr)) < 0) {
        perror("bind()");
        exit(1);
    }

    raddr_len = sizeof(raddr); //很重要 为第一次传输初始化
    while (1) {
        recvfrom(sd, &rbuf, sizeof(rbuf), 0, (void*)&raddr, &raddr_len);
        inet_ntop(AF_INET, &raddr.sin_addr, ipstr, IPSTRSIZE);
        printf("-----MESSAGE FROM %s:%d-----\n", ipstr, ntohs(raddr.sin_port));
        printmsg(&rbuf);
    }

    close(sd);

    exit(0);
}
