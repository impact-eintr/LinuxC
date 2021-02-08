#ifndef _RELAY_H_H
#define _RELAY_H_H

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#define TTY1 "/dev/tty11"
#define TTY2 "/dev/tty12"
#define TTY3 "/dev/tty13"
#define TTY4 "/dev/tty14"
#define BUFSIZE 1024
#define REL_JOBMAX 10000


enum
{
    STATE_R = 1,//running
    STATE_C,    //cancel
    STATE_O     //over
};

struct rel_stat_st{
    int state;
    int fd1;
    int fd2;
    int64_t count12,count21;//计数器

};

/*
 *return >= 0 成功返回当前任务ID
         == -EINVAL 参数非法
         == -ENOSPC 
         == -ENOMEM 内存操作失败
 * */
int rel_addjob(int fd1,int fd2);

/*
 *return =0 成功
 == -EINVAL 参数非法
 == -EBUSY 取消过了
 * */
int rel_canceljob(int jo0b);

/*收尸
 * return == 0测横供指定任务已终止并返回状态
 *      == -EINVAL       
 * */
int rel_waitjob(int id,struct rel_stat_st *);;

int rel_statjob(int id,struct rel_stat_st *);

#endif
