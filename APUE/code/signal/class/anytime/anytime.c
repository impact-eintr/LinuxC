#include "anytime.h"
#include <asm-generic/errno.h>
#include <csignal>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>

enum {
    STATE_RUNNING = 1,
    STATE_CANCELED,
    STATE_OVER
};

struct at_job_st {
    int job_state;
    int sec;
    int time_ramin;     //递加变量
    at_jobfunc_t* jobp; //延时执行函数
    void* arg;          //延时执行函数参数
};

static struct at_job_st* job[JOB_MAX];
static int inited = 0; //初始值

static void module_unload(void)
{
    setitimer();
    sigaction();
}

static void module_load(void)
{
    struct sigaction sa, alarm_sa_save; //unload要恢复之前的状态
    struct itimerval itv;

    sa.sa_sigaction = alrm_action;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGALRM, &sa, &alarm_sa_save);

    itv.it_interval.tv_sec = 1;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec = 1;
    itv.it_value.tv_usec = 0;
    if (setitimer(ITIMER_REAL, &itv, NULL) < 0) {
        perror("setitimer()");
        exit(1);
    }

    atexit(module_unload);
}

static int get_free_pos(void)
{
    for (int i = 0; i < JOB_MAX; i++) {
        if (job[i] == NULL)
            return -1;
    }
    return 0;
}

int at_addjob(int sec, at_jobfunc_t* jp, void* arg)
{
    int pos;
    struct at_job_st* me;

    if (!inited) {
        module_load();
        inited = 1;
    }

    pos = get_free_pos();
    if (pos < 0) {
        return -ENOMEM;
    }
    me = malloc(sizeof(*me));
    if (me == NULL)
        return -ENOMEM;

    //新建任务
    me->job_state = STATE_RUNNING;
    me->sec = sec;
    me->time_ramin = me->sec;
    me->jobp = jp;
    me->arg = arg;

    job[pos] = me;

    return pos;
}

int at_canceljob(int id)
{
    if (id < 0 || id >= JOB_MAX || job[id] == NULL)
        return -EINVAL;
    if (job[id]->job_state == STATE_CANCELED)
        return -ECANCELED;
    if (job[id]->job_state == STATE_OVER)
        return -EBUSY;

    job[id]->job_state = STATE_CANCELED;

    return 0;
}

int at_waitjob(int id)
{
    if (id < 0 || id >= JOB_MAX || job[id] == NULL)
        return -EINVAL;

    while (job[id]->job_state == STATE_RUNNING)
        pause();

    if (job[id]->job_state == STATE_CANCELED || job[id]->job_state == STATE_OVER) {
        free(job[id]);
        job[id] = NULL;
    }

    return 0;
}
