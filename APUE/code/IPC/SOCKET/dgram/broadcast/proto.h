#ifndef PROTO_H__
#define PROTO_H__

#include <stdint.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define NAMESIZE 1024
#define RCVPORT "1989"

struct msg_st{
    char name[NAMESIZE];
    uint32_t math;
    uint32_t chinese;
}__attribute__((packed));//不对齐


#endif
