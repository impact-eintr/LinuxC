#ifndef PROTO_H__
#define PROTO_H__

#include <stdint.h>

#define NAMESIZE 128

struct msg_st{
    uint8_t name[NAMESIZE];
    uint32_t math;
    uint32_t chinese
}__attribute__((packed));//不对齐

#endif
