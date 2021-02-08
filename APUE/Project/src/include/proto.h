#ifndef PROTO_H__
#define PROTO_H__

#include "site_type.h"

#define DEFAULT_MGOUP       "224.2.2.2"//多播组 b类地址
#define DEFAULT_RCVPROT     "1989"//通讯端口

#define CHANNELNUM          100//定义总频道个数

#define LISTCHANNELNUMID     0//0号频道发送节目单

#define MINCHANNELNUMID     1
#define MAXCHANNELNUMID     (MINCHANNELNUMID+CHANNELNUM-1)

#define MSG_CHANNEL_MAX     (65536-20-8)//udp推荐包长度-ip报头长度-udp报头长度
#define MAX_DATA            (MSG_CHANNEL_MAX - sizeof(chnid_t))//推荐最大数据包上限

#define MSG_LIST_MAX        (65536-20-8)//udp推荐包长度-ip报头长度-udp报头长度
#define MAX_ENTRY           (MSG_LIST_MAX - sizeof(chnid_t))//推荐最大数据包上限




struct msg_channel_st{
    chnid_t chnid;      //[MINCHANNELNUMID,MAXCHANNELNUMID]
    uint8_t data[1];//变长结构体
}__attribute__((packed));


struct msg_listentry_st{//entry 条目
    chnid_t chnid;      //[1,MAXCHANNELNUMID]
    uint16_t len;//该结构体的大小
    uint8_t desc[1];//变长结构体,节目单描述
}__attribute__((packed));

struct msg_list_st{//节目单
    chnid_t chnid;      // LISTCHNNELNUMID
    struct msg_listentry_st entry[1];
}__attribute__((packed));




#endif
