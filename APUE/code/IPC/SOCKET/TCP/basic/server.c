#include "proto.h"
#include <asm-generic/socket.h>
#include <stdio.h>
#include <stdlib.h>

#define IPSIZE 40
#define BUFSIZE 1024

static void serversend(int sd)
{
    int len;
    char buf[BUFSIZE];
    len = sprintf(buf, FMT_STAMP, (long long)time(NULL));

    if (send(sd, buf, len, 0) < 0) {
        perror("send()");
        exit(1);
    }
}

int main()
{
    int sd, newsd;
    struct sockaddr_in laddr, raddr;
    socklen_t raddr_len;
    char ipstr[IPSIZE];

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0) {
        perror("socket()");
        exit(1);
    }

    int val = 1;
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) < 0) {
        perror("setsocketopt()");
        exit(1);
    }

    laddr.sin_family = AF_INET;
    laddr.sin_port = htons(atoi(SERVERPORT));
    inet_pton(AF_INET, "0.0.0.0", &laddr.sin_addr);
    if (bind(sd, (void*)&laddr, sizeof(laddr)) < 0) {
        perror("bind()");
        exit(1);
    }

    if (listen(sd, 200) < 0) {
        perror("listen()");
        exit(1);
    }

    raddr_len = sizeof(raddr);
    while (1) {

        newsd = accept(sd, (void*)&raddr, &raddr_len);
        if (newsd < 0) {
            perror("accept()");
            exit(1);
        }

        inet_ntop(AF_INET, &raddr.sin_addr, ipstr, IPSIZE);
        printf("Client:%s:%d\n", ipstr, ntohs(raddr.sin_port));

        serversend(newsd);
        close(newsd);
    }

    close(sd);

    exit(0);
}
