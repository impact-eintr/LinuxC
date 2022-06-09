#include <errno.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>

#include "../include/proto.h"
#include "medialib.h"
#include "server_conf.h"
#include "thr_list.h"

static pthread_t tid_list; // 线程列表
static int num_list_entry;
static struct mlib_listentry_st *list_entry; // 频道列表

static void *thr_list(void *p) {
  int totalsize;
  struct msg_list_st *entrylistptr;
  struct msg_listentry_st *entryptr;
  int ret;
  int size;

  totalsize = sizeof(chnid_t); // 之后逐步累计
  for (int i = 0; i < num_list_entry; ++i) {
    totalsize += sizeof(struct msg_listentry_st) + strlen(list_entry[i].desc);
  }
  entrylistptr = malloc(totalsize);
  if (entrylistptr == NULL) {
    syslog(LOG_ERR, "malloc():%s", strerror(errno));
    exit(1);
  }
  entrylistptr->chnid = LISTCHNID; // 这是列表频道

  entryptr = entrylistptr->entry;
  syslog(LOG_DEBUG, "nr_list_entn:%d\n", num_list_entry);
  for (int i = 0; i < num_list_entry; ++i) {
    size = sizeof(struct msg_listentry_st) + strlen(list_entry[i].desc);

    entryptr->chnid = list_entry[i].chnid;
    entryptr->len = htons(size);
    strcpy(entryptr->desc, list_entry[i].desc);
    entryptr = (void *)(((char *)entryptr) + size); // 向后移动entptr
    syslog(LOG_DEBUG, "entryp len:%d\n", entryptr->len);
  }

  while (1) {
    syslog(LOG_INFO, "thr_list sndaddr :%d\n", sndaddr.sin_addr.s_addr);
    ret = sendto(serversd, entrylistptr, totalsize, 0, (void *)&sndaddr,
                 sizeof(sndaddr)); // 频道列表在广播网段每秒发送entrylist
    syslog(LOG_DEBUG, "sent content len:%d\n", entrylistptr->entry->len);
    if (ret < 0) {
      syslog(LOG_WARNING, "sendto(serversd, enlistp...:%s", strerror(errno));
    } else {
      syslog(LOG_DEBUG, "sendto(serversd, enlistp....):success");
    }
    sleep(1);
  }
}

// 创建节目单线程
int thr_list_create(struct mlib_listentry_st *listptr, int num_ent) {
  int err;
  list_entry = listptr;
  num_list_entry = num_ent;
  syslog(LOG_DEBUG, "list content: chnid:%d, desc:%s\n", listptr->chnid,
         listptr->desc);
  err = pthread_create(&tid_list, NULL, thr_list, NULL);
  if (err) {
    syslog(LOG_ERR, "pthread_create():%s", strerror(errno));
    return -1;
  }
  return 0;
}

// 销毁节目单线程
int thr_list_destroy(void) {
  pthread_cancel(tid_list);
  pthread_join(tid_list, NULL);
  return 0;
}
