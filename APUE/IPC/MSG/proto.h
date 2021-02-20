#ifndef PROTO_H__
#define PROTO_H__

#define KEYPATH "/etc/services"
#define KEYPROJ 'g'
#define NAMESIZE 1024

struct msg_st{
    long mtype;
    char name[NAMESIZE];
    int math;
    int chinese;
};


#endif

