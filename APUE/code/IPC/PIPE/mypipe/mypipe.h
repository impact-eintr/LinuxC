#ifndef MYPIPE_H__
#define MYPIPE_H__
#include <pthread.h>

#define PIPESIZE 4096
#define MYPIPE_READ 0x00000001UL
#define MYPIPE_WRITE 0x00000002UL

typedef void pipe_t;

pipe_t *pipeinit(void);

int pipe_read(pipe_t *ptr,void *buf,size_t size);

//注册
int pipe_register(pipe_t *,int opmap);//opmap操作位图

//注销
int pipe_unregister(pipe_t *,int opmap);//opmap操作位图

int pipe_write(pipe_t *,const void *,size_t);

int pipe_destory(pipe_t *);


#endif
