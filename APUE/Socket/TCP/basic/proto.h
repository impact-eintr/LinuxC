#ifndef PROTO_H__
#define PROTO_H__

#include <stdint.h>

#define NAMEMAX 512-8-8//(UDP推荐长度-UDP报头长度-结构体的长度)
#define FMT_STAMP "%lld\n"
#define SERVERPORT "2333"

struct msg_st{
    long long stamp;
}__attribute__((packed));//不对齐

#endif

