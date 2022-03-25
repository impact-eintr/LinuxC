#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>

#include "mytbf.h"

static int min(int a, int b) { return a < b ? a : b; }

struct mytbf_st {
  int cps; // c per second
  int burst;
  int token;
  int pos;
  pthread_mutex_t mut;
  pthread_cond_t cond;
};

static struct mytbf_st *job[MYTBF_MAX];
static pthread_mutex_t mut_job = PTHREAD_MUTEX_INITIALIZER;

static pthread_once_t once_init = PTHREAD_ONCE_INIT;
static pthread_t tid;

static void alrm_handle(int sig) {
  pthread_mutex_lock(&mut_job);
    for (int i = 0; i < MYTBF_MAX; ++i) {
      if (job[i] != NULL) {
        pthread_mutex_lock(&job[i]->mut);
        job[i]->token += job[i]->cps;
        if (job[i]->token > job[i]->burst) {
          job[i]->token = job[i]->burst;
        }
        pthread_cond_broadcast(&job[i]->cond); // 惊群
        pthread_mutex_unlock(&job[i]->mut);
      }
    }
    pthread_mutex_unlock(&mut_job);
}

static void *thr_alrm(void *p) {
  struct itimerval tick;
  memset(&tick, 0, sizeof(tick));
  tick.it_value.tv_sec = 1;  // sec
  tick.it_value.tv_usec = 0; // micro sec.
  tick.it_interval.tv_sec = 1;
  tick.it_interval.tv_usec = 0;

  signal(SIGALRM, alrm_handle);
  setitimer(ITIMER_REAL, &tick, NULL);

  while (1) {
    pause();
  }
}

// 模块卸载函数
static void module_unload() {
  int i;
  pthread_cancel(tid);
  pthread_join(tid, NULL);

  for (int i = 0; i < MYTBF_MAX; i++) {
    free(job[i]);
  }
  return;
}

// 模块加载函数
static void module_load() {
  int err;
  err = pthread_create(&tid, NULL, thr_alrm, NULL);
  if (err) {
    fprintf(stderr, "pthread_create():%s", strerror(errno));
    exit(1);
  }
  atexit(module_unload);
}

static int get_free_pos_unlocked() {
  for (int i = 0; i < MYTBF_MAX; ++i) {
    if (job[i] == NULL) {
      return i;
    }
  }
  return -1;
}

// 初始化一个令牌桶
mytbf_t *mytbf_init(int cps, int burst) {
  struct mytbf_st *me;

  module_load();                         // 开启定时token派发
  pthread_once(&once_init, module_load); // 限定只开启一次

  int pos;
  // 初始化mytbf
  me = malloc(sizeof(*me));
  if (me == NULL) {
    return NULL;
  }
  me->cps = cps;
  me->burst = burst;
  me->token = 0;
  pthread_mutex_init(&me->mut, NULL); // 初始化该令牌桶的mutex
  pthread_cond_init(&me->cond, NULL); // 初始化该令牌桶的conditional variable
  pthread_mutex_lock(&mut_job);

  pos = get_free_pos_unlocked();
  if (pos < 0) {
    pthread_mutex_unlock(&mut_job);
    fprintf(stderr, "no free position,\n");
    free(me);
    exit(1);
  }
  me->pos = pos;
  job[me->pos] = me; // 分配槽位

  pthread_mutex_unlock(&mut_job);
  return me;
}

int mytbf_fetchtoken(mytbf_t *ptr, int size) {
  int n;
  struct mytbf_st *me = ptr;
  pthread_mutex_lock(&me->mut);
  while (me->token <= 0)
    pthread_cond_wait(&me->cond, &me->mut); // 没有令牌的时候 等待信号量通知
  n = min(me->token, size);
  me->token -= n;
  pthread_cond_broadcast(&me->cond);
  pthread_mutex_unlock(&me->mut);
  return n;
}

int mytbf_returntoken(mytbf_t *ptr, int size) {
  struct mytbf_st *me = ptr;
  pthread_mutex_lock(&me->mut);
  me->token += size;
  if (me->token > me->burst)
    me->token = me->burst;
  pthread_mutex_unlock(&me->mut);
  return 0;
}

int mytbf_destroy(mytbf_t *ptr) {
  struct mytbf_st *me = ptr;
  pthread_mutex_lock(&mut_job);
  job[me->pos] = NULL;
  pthread_mutex_unlock(&mut_job);

  pthread_mutex_destroy(&me->mut);
  pthread_cond_destroy(&me->cond);
  free(ptr);
  return 0;
}

int mytbf_checktoken(mytbf_t *ptr) {
  int token_left = 0;
  struct mytbf_st *me = ptr;
  pthread_mutex_lock(&me->mut);
  token_left = me->token;
  pthread_mutex_unlock(&me->mut);
  return token_left;
}
