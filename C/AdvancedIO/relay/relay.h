#ifndef RELAY_H__
#define RELAY_H__

#include <stdlib.h>

#define JOBMAX 10000

enum {
    STATE_RUNNING = 1,
    STATE_CANCELD,
    STATE_OVER
};

struct relay_stat_st{
    int state;
    int fd1;
    int fd2;
    int64_t count12,count21;
};

//return >= 0 get job id
//       == -EINVAL 参数非法
//       == -ENOSPC 任务数组满
//       == -ENOMEM 内存分配出错
int relay_addjob(int fd1,int fd2);

//return == 0 取消成功
//       == -EINVAL 单数非法
//       == -EBUSY 任务取消过了
int relay_canceljob(int id);

//return == 0 返回状态
//       == -EINVAL 参数非法
int relay_waitjob(int id);

//reutnr == 0 指定任务状态已返回
int relay_statjob(int id,struct relay_stat_st *);

#endif
