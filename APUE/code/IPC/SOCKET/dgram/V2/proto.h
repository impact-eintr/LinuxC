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

#define NAMEMAX (512-8-8)
#define RCVPORT "1989"

struct msg_st{
    uint32_t math;
    uint32_t chinese;
    char name[1];
}__attribute__((packed));//不对齐


#endif
