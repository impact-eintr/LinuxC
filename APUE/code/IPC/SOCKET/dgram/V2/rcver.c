#include "proto.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#define IPSTRSIZE 40
#if 0
static void printmsg(msg_st* msg)
{
    printf(("MANE = %s\n", *msg.name));
    printf("MATH = %d\n", ntohl(*msg.math));
    printf("CHINESE = %d\n", ntohl(*msg.chinese));
}
#endif
int main()
{
    int sd, size;
    struct sockaddr_in laddr, raddr; //l local r remote遥远的
    char ipstr[IPSTRSIZE];
    struct msg_st* rbufp;
    socklen_t raddr_len;
    size = sizeof(struct msg_st) + NAMEMAX - 1; //name[1]
    rbufp = malloc(size);
    if (rbufp == NULL) {
        perror("malloc()");
        exit(1);
    }
    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0) {
        perror("socket");
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
        recvfrom(sd, rbufp, size, 0, (void*)&raddr, &raddr_len);
        inet_ntop(AF_INET, &raddr.sin_addr, ipstr, IPSTRSIZE);
        printf("-----MESSAGE FROM %s:%d-----\n", ipstr, ntohs(raddr.sin_port));
        printf("NAME = %s\n", rbufp->name);
        printf("MATH = %d\n", ntohl(rbufp->math));
        printf("CHINESE = %d\n", ntohl(rbufp->chinese));
    }

    close(sd);

    exit(0);
}
