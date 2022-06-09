#ifndef PROTO_H_
#define PROTO_H_

#include <stdint.h>

#define DEFAULT_MGROUP "224.2.2.2" // default multicast group
#define DEFAULT_RCVPORT "1989"
#define CHANNUM 200 // channel number

#define LISTCHNID 0 // list channel
#define MINCHNID 1 // minimum channel id
#define MAXCHNID (MINCHNID + CHANNUM - 1) // maximum channel id

#define MSG_CHANNEL_MAX ((1<<16)-20-8) // 20:IP package head, 8:udp package head
#define MAX_DATA (MSG_CHANNEL_MAX - sizeof(chnid_t))

#define MSG_LIST_MAX ((1<<16)-20-8)
#define MAX_DATA (MSG_CHANNEL_MAX - sizeof(chnid_t))

#include "site_type.h"
// 每一个频道内容结构体： 频道号，data(指针)
struct msg_channel_st
{
  chnid_t chnid; // must between MINCHNID MAXCHNID
  uint8_t data[1];
}__attribute__((packed)); // do not align

// 每一条节目单包含的信息：chnid len desc
struct msg_listentry_st
{
  chnid_t chnid;
  uint16_t len;
  char desc[1]; // 频道的描述信息
  //uint8_t desc[1]; // 频道的描述信息
}__attribute__((packed)); // do not align

// 节目单频道内容 chnid len desc
struct msg_list_st
{
  chnid_t chnid; // must be LISTCHNID
  struct msg_listentry_st entry[1];
}__attribute__((packed)); // do not align

#endif // PROTO_H_
