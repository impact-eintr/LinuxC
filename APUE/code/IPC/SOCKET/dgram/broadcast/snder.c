#include "proto.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

int main(int argc, char** argv)
{
    int sd;
    struct msg_st sbuf;
    struct sockaddr_in raddr; //r remote遥远的

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0) {
        perror("socket()");
        exit(1);
    }
    int val = 1;
    if (setsockopt(sd, SOL_SOCKET, SO_BROADCAST, &val, sizeof(val))) {
        perror("socket()");
        exit(1);
    }
    strcpy(sbuf.name, "Mike");
    sbuf.math = htonl(rand() % 100);
    sbuf.chinese = htonl(rand() % 100);

    raddr.sin_family = AF_INET;
    raddr.sin_port = htons(atoi(RCVPORT));
    inet_pton(AF_INET, "255.255.255.255", &raddr.sin_addr);

    if (sendto(sd, &sbuf, sizeof(sbuf), 0, (void*)&raddr, sizeof(raddr)) < 0) {
        perror("sendto()");
        exit(1);
    }

    puts("OJBK!");
    close(sd);

    exit(0);
}
