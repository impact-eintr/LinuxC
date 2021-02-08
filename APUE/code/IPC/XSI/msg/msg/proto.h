#ifndef PROTO_H___
#define PROTO_H___

#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

//协议文件
#define KEYPATH "/etc/services"
#define KEYPROJ 'g'
#define NAMESIZE 1024

struct msg_st{
    long mytype;
    char name[NAMESIZE];
    int math;
    int chinese;
};

#endif
