#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <errno.h>

#define BUFFSIZE 150

int main()
{
    struct sockaddr_in serv;
    char buff[BUFFSIZE];
    int sockfd, n;

    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket()");
        exit(1);
    }

}
