#ifndef ANYTIMER_H__
#define ANYTIMER_H__

#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>

#define JOB_MAX 1024
typedef void at_jobfunc_t(void*);

#if 0
return  >= 0       成功 返回任务ID
== -EINVAL 失败 参数非法
== -ENOSPC 失败 数组满
== -ENOMEM 失败 内存空间不足
#endif
int at_addjob(int sec,at_jobfunc_t *jp,void *arg);

#if 0
return  >= 0       成功 指定任务取消成功
== -EINVAL 失败 参数非法
== -EBUSY 失败 指定任务已完成
== -ECANCLED 失败 重复取消
#endif
int at_canceljob(int id);

/*
   return  >= 0       成功 返回任务ID
   == -EINVAL 失败 参数非法
   */
int at_waitjob(int id);



#endif
