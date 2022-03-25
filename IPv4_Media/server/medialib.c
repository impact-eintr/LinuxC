#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>

#include "../include/proto.h"
#include "medialib.h"
#include "mytbf.h"
#include "server_conf.h"

//#define DEBUG

#define PATHSIZE 1024
#define LINEBUFSIZE 1024
#define MP3_BITRATE 320 * 1024 // 采样率

struct channel_context_st {
  chnid_t chnid;
  char *desc;
  glob_t mp3glob; // 目录项
  int pos;        // current song
  int fd;         // current song fd
  off_t offset;
  mytbf_t *tbf; // 流控器
};

static struct channel_context_st channel[MAXCHNID + 1]; // 全部的频道

// 将某个目录下的所有文件转为一个频道
static struct channel_context_st *path2entry(const char *path) {
  syslog(LOG_INFO, "current path: %s\n", path);
  char pathstr[PATHSIZE] = {'\0'};
  char linebuf[LINEBUFSIZE];
  FILE *fp;
  struct channel_context_st *me;

  static chnid_t curr_id =
      MINCHNID; // 由于是一个静态变量所以相当于一直在操作同一块内存 有叠加效果

  // 检测目录合法性
  strcat(pathstr, path);
  strcat(pathstr, "/desc.txt");
  fp = fopen(pathstr, "r"); // 打开频道描述文件
  syslog(LOG_INFO, "channel dir:%s\n", pathstr);
  if (fp == NULL) {
    syslog(LOG_INFO, "%s is not a channe; dir (can not find desc.txt)", path);
    return NULL;
  }
  if (fgets(linebuf, LINEBUFSIZE, fp) == NULL) {
    syslog(LOG_INFO, "%s is not a channel dir(cant get the desc.text)", path);
    fclose(fp);
    return NULL;
  }
  fclose(fp); // 关闭频道描述文件

  // 初始化上下文
  me = malloc(sizeof(*me));
  if (me == NULL) {
    syslog(LOG_ERR, "malloc():%s", strerror(errno));
    return NULL;
  }

  me->tbf = mytbf_init(MP3_BITRATE / 8, MP3_BITRATE / 8 * 5); // 初始化流控器
  if (me->tbf == NULL) {
    syslog(LOG_ERR, "mytbf_init():%s", strerror(errno));
    free(me);
    return NULL;
  }

  // 初始化频道
  me->desc = strdup(linebuf);
  strncpy(pathstr, path, PATHSIZE);
  strncat(pathstr, "/*.mp3", PATHSIZE-1);
  if (glob(pathstr, 0, NULL, &me->mp3glob) != 0) {
    curr_id++;
    syslog(LOG_ERR, "%s is not a channel dir(can not find mp3 files", path);
    free(me);
    return NULL;
  }
  me->pos = 0;
  me->offset = 0;
  me->fd = open(me->mp3glob.gl_pathv[me->pos], O_RDONLY); // 打开第一个音乐文件
  if (me->fd < 0) {
    syslog(LOG_WARNING, "%s open failed.", me->mp3glob.gl_pathv[me->pos]);
    free(me);
    return NULL;
  }
  me->chnid = curr_id;
  curr_id++;
  return me;
}

int mlib_getchnlist(struct mlib_listentry_st **result, int *resnum) {
  int num = 0;
  int i = 0;
  char path[PATHSIZE];
  glob_t globres;
  struct mlib_listentry_st *ptr;
  struct channel_context_st *res;

  for (int i = 0; i < MAXCHNID; ++i) {
    channel[i].chnid = -1;
  }

  snprintf(path, PATHSIZE, "%s/*", server_conf.media_dir);
#ifdef DEBUG
  printf("current path:%s\n", path);
#endif
  if (glob(path, 0, NULL, &globres)) { // 成功返回0
    return -1;
  }
#ifdef DEBUG
  printf("globres.gl_pathv[0]:%s\n", globres.gl_pathv[0]);
  printf("globres.gl_pathv[1]:%s\n", globres.gl_pathv[1]);
  printf("globres.gl_pathv[2]:%s\n", globres.gl_pathv[2]);
#endif
  ptr = malloc(sizeof(struct mlib_listentry_st) * globres.gl_pathc);
  if (ptr == NULL) {
    syslog(LOG_ERR, "malloc() error.");
    exit(1);
  }
  for (i = 0; i < globres.gl_pathc; ++i) {
    res = path2entry(globres.gl_pathv[i]);
    if (res != NULL) {
      syslog(LOG_ERR, "path2entry() return : %d %s.", res->chnid, res->desc);
      memcpy(channel + res->chnid, res, sizeof(*res));
      ptr[num].chnid = res->chnid;
      ptr[num].desc = strdup(res->desc);
      num++;
    }
  }
  *result = realloc(ptr, sizeof(struct mlib_listentry_st) * num);
  if (*result == NULL) {
    syslog(LOG_ERR, "realloc() failed.");
  }
  *resnum = num;
  return 0;
}

int mlib_freechnlist(struct mlib_listentry_st *ptr) {
  free(ptr);
  return 0;
}

static int open_next(chnid_t chnid) {
  for (int i = 0; i < channel[chnid].mp3glob.gl_pathc; ++i) {
    channel[chnid].pos++; // 更新偏移
    if (channel[chnid].pos == channel[chnid].mp3glob.gl_pathc) {
      printf("没有新文件了 列表循环\n");
      channel[chnid].pos = 0;
    }
    close(channel[chnid].fd);

    // 尝试打开新文件
    channel[chnid].fd =
        open(channel[chnid].mp3glob.gl_pathv[channel[chnid].pos], O_RDONLY);
    if (channel[chnid].fd < 0) {
      syslog(LOG_WARNING, "open(%s):%s", channel[chnid].mp3glob.gl_pathv[chnid],
             strerror(errno));
    } else {
      printf("打开新文件了\n");
      channel[chnid].offset = 0;
      return 0;
    }
  }
  syslog(LOG_ERR, "None of mp3 in channel %d id available.", chnid);
  return -1;
}

ssize_t mlib_readchn(chnid_t chnid, void *buf, size_t size) {
  int tbfsize;
  int len;
  int next_ret = 0;
  // get token number
  tbfsize = mytbf_fetchtoken(channel[chnid].tbf, size);
  syslog(LOG_INFO, "current tbf():%d", mytbf_checktoken(channel[chnid].tbf));

  while (1) {
    len = pread(channel[chnid].fd, buf, tbfsize, channel[chnid].offset); // 读取tbfsize数据到从offset处开始的buf
    /*current song open failed*/
    if (len < 0) {
      // 当前这首歌可能有问题，错误不至于退出，读取下一首歌
      syslog(LOG_WARNING, "media file %s pread():%s",
             channel[chnid].mp3glob.gl_pathv[channel[chnid].pos],
             strerror(errno));
      open_next(chnid);
    } else if (len == 0) {
      syslog(LOG_DEBUG, "media %s file is over",
             channel[chnid].mp3glob.gl_pathv[channel[chnid].pos]);
#ifdef DEBUG
      printf("current chnid :%d\n", chnid);
#endif
      next_ret = open_next(chnid);
      break;
    } else /*len > 0*/ //真正读取到了数据
    {
      channel[chnid].offset += len;
      struct stat buf;
      fstat(channel[chnid].fd, &buf);
      syslog(LOG_DEBUG, "epoch : %f%%",
             (channel[chnid].offset) / (1.0*buf.st_size)*100);
      break;
    }
  }
  // remain some token
  if (tbfsize - len > 0)
    mytbf_returntoken(channel[chnid].tbf, tbfsize - len);
  printf("current chnid :%d\n", chnid);
  return len; //返回读取到的长度
}
