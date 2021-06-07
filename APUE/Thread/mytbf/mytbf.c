#include <errno.h>
#include <sched.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>

#include "mytbf.h"

struct mytbf_st{
  int csp;
  int burst;
  int token;
  int pos;//任务列表的下标
  pthread_mutex_t mut;
  pthread_cond_t cond;
};

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static pthread_t ptid;
static pthread_once_t pth_once = PTHREAD_ONCE_INIT;

static struct mytbf_st *job[MYTBF_MAX];
static volatile int inited = 0;

static int get_free_pos_unlocked(){
  for (int i = 0;i < MYTBF_MAX;i++){
    if (job[i] == NULL)
      return  i;
  }
  return -1;
}

//线程处理函数
static void *handler(void *p){
  struct timespec ts;
  ts.tv_sec = 1;
  ts.tv_nsec = 0;

  while(1){
    pthread_mutex_lock(&mutex);
    for (int i = 0;i < MYTBF_MAX;i++){
      if (job[i] != NULL){
        pthread_mutex_lock(&job[i]->mut);
        job[i]->token += job[i]->csp;
        if (job[i]->token > job[i]->burst){
          job[i]->token = job[i]->burst;
        }
        pthread_cond_broadcast(&job[i]->cond);
        pthread_mutex_unlock(&job[i]->mut);
      }
    }
    pthread_mutex_unlock(&mutex);
    nanosleep(&ts,NULL);

  }
  pthread_exit(NULL);
}

//卸载线程处理模块
static void mod_unload(){
  pthread_cancel(ptid);
  pthread_join(ptid,NULL);
  for (int i = 0;i < MYTBF_MAX;i++){
    if (job[i] != NULL){
      mytbf_destroy(job[i]);
    }
    free(job[i]);
  }

  pthread_mutex_destroy(&mutex);
}

//装载线程处理模块
static void mod_load(){

  int err = pthread_create(&ptid,NULL,handler,NULL);
  if (err){
    fprintf(stderr,"%s\n",strerror(err));
  }

  atexit(mod_unload);
}

mytbf_t *mytbf_init(int cps,int burst){
  struct mytbf_st *tbf;

  pthread_once(&pth_once,mod_load);

  tbf = malloc(sizeof(*tbf));
  if (tbf == NULL){
    return NULL;
  }
  tbf->token = 0;
  tbf->csp = cps;
  tbf->burst = burst;
  pthread_mutex_init(&tbf->mut,NULL);
  pthread_cond_init(&tbf->cond,NULL);

  pthread_mutex_lock(&mutex);
  //将新的tbf装载到任务组中
  int pos = get_free_pos_unlocked();
  if (pos == -1){
    free(tbf);
    pthread_mutex_unlock(&mutex);
    return NULL;
  }

  tbf->pos = pos;
  job[pos] = tbf;
    
  pthread_mutex_unlock(&mutex);

  return tbf;
}

//获取token ptr是一个 void * size是用户想要获取的token数
int mytbf_fetchtoken(mytbf_t *ptr,int size){
  struct mytbf_st *tbf = ptr;

  if (size <= 0){
    return -EINVAL;
  }
    
  //有token继续
  pthread_mutex_lock(&tbf->mut);
  while (tbf->token <= 0){
    pthread_cond_wait(&tbf->cond,&tbf->mut);//等通知 抢锁
  }

  int n =tbf->token<size?tbf->token:size;
  tbf->token -= n;

  pthread_mutex_unlock(&tbf->mut);
  //用户获取了 n 个token
  return n;
}

//归还token ptr是一个 void *
int mytbf_returntoken(mytbf_t *ptr,int size){
  struct mytbf_st *tbf = ptr;

  if (size <= 0){
    return -EINVAL;
  }
  pthread_mutex_lock(&tbf->mut);
  tbf->token += size;
  if (tbf->token > tbf->burst)
    tbf->token = tbf->burst;
  pthread_cond_broadcast(&tbf->cond);
  pthread_mutex_unlock(&tbf->mut);

  return size;
}

int mytbf_destroy(mytbf_t *ptr){
  struct mytbf_st *tbf = ptr;
  pthread_mutex_lock(&mutex);
  job[tbf->pos] = NULL;
  pthread_mutex_unlock(&mutex);

  pthread_mutex_destroy(&tbf->mut);
  pthread_cond_destroy(&tbf->cond);

  free(tbf);
  return 0;
}
