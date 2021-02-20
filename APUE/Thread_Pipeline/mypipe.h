#ifndef MYPIPE_H__
#define MYPIPE_H__

#include <stdio.h>

#define PIPESIZE 1024
#define PIPE_READER 0x00000001UL //读者
#define PIPE_WRITER 0x00000002UL //写者


typedef void mypipe_t;

mypipe_t *mypipe_init();

//读者 写者 注册身份
int mypipe_register(mypipe_t *,int opmap);;
//读者 写者 注销身份
int mypipe_unregister(mypipe_t *,int opmap);

int mypipe_read(mypipe_t *,void *buf,size_t size);

int mypipe_write(mypipe_t *,const void *buf,size_t size);

int mypipe_destory(mypipe_t *);

#endif
