#include "mytbf.h"
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <syslog.h>
#include <unistd.h>

struct mytbf_st {
    int cps;
    int burst;
    int token;
    int pos;
    pthread_mutex_t mut; //互斥
    pthread_cond_t cond; //通知
};

static struct mytbf_st* job[MYTBF_MAX];                     //令牌桶任务数组
static pthread_mutex_t mut_job = PTHREAD_MUTEX_INITIALIZER; //初始化任务互斥量
static pthread_once_t init_once;

static pthread_t tid;

static void* thr_alrm(void* p)
{
    while (1) {
        pthread_mutex_lock(&mut_job);
        for (int i = 0; i < MYTBF_MAX; i++) {
            if (job[i] != NULL) {
                pthread_mutex_lock(&job[i]->mut);
                job[i]->token += job[i]->cps;
                if (job[i]->token > job[i]->burst)
                    job[i]->token = job[i]->burst;
                pthread_cond_broadcast(&job[i]->cond);
                pthread_mutex_unlock(&job[i]->mut);
            }
        }
        pthread_mutex_unlock(&mut_job);
        sleep(1);
    }
    return NULL;
}
//解除令牌桶
static void moude_unload(void)
{
    pthread_cancel(tid);
    pthread_join(tid, NULL);

    for (int i = 0; i < MYTBF_MAX; i++)
        free(job[i]);

    pthread_mutex_destroy(&mut_job);
}
//加载令牌桶机制
static void moude_load(void)
{
    int err;

    err = pthread_create(&tid, NULL, thr_alrm, NULL);
    if (err) {
        fprintf(stderr, "pthread_create():%s\n", strerror(errno));
        exit(1);
    }
    atexit(moude_unload);
}

static int get_free_pos_unlocked()
{
    int i;
    for (i = 0; i < MYTBF_MAX; i++) {
        if (job[i] == NULL)
            return i;
    }
    return -1;
}

mytbf_t* mytbf_init(int cps, int burst)
{

    struct mytbf_st* me;
    int pos;
    pthread_once(&init_once, moude_load);

    me = malloc(sizeof(*me));
    me->cps = cps;
    me->burst = burst;
    me->token = 0;
    pthread_mutex_init(&me->mut, NULL);
    pthread_cond_init(&me->cond, NULL);

    pthread_mutex_lock(&mut_job); //开始原子操作job数组，锁住互斥量
    pos = get_free_pos_unlocked();
    if (pos < 0) {
        fprintf(stderr, " get_free_pos_unlocked() faild\n");
        pthread_mutex_unlock(&mut_job);
        free(me);
        return NULL;
    }
    me->pos = pos;
    job[me->pos] = me;
    pthread_mutex_unlock(&mut_job);
    return me;
}

int mytbf_fetchtoke(mytbf_t* ptr, int size)
{
    struct mytbf_st* me = ptr;

    pthread_mutex_lock(&me->mut);

    while (me->token < size)
        pthread_cond_wait(&me->cond, &me->mut);

    int n = me->token <= size ? me->token : size;
    me->token -= n;
    pthread_mutex_unlock(&me->mut);

    return n;
}

int mytbf_returntoke(mytbf_t* ptr, int size)
{
    struct mytbf_st* me = ptr;

    pthread_mutex_lock(&me->mut);
    me->token += size;
    if (me->token > me->burst)
        me->token = me->burst;

    pthread_cond_broadcast(&me->cond);
    pthread_mutex_unlock(&me->mut);

    return 0;
}

int mytbf_destroy(mytbf_t* ptr)
{
    struct mytbf_st* me = ptr;
    //先释放数组资源，使得其他任务可以获取资源
    pthread_mutex_lock(&mut_job);
    job[me->pos] = NULL;
    pthread_mutex_unlock(&mut_job);
    //释放当前任务自有资源
    pthread_mutex_unlock(&me->mut);
    pthread_cond_destroy(&me->cond);
    free(ptr);
    return 0;
}
