#include "mytbf.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct mytbf_st {
  int cps;
  int burst;
  int token;
  int pos;
  pthread_mutex_t mut;
};

static struct mytbf_st *job[MYTBF_MAX];
static pthread_mutex_t mut_job = PTHREAD_MUTEX_INITIALIZER;
static pthread_once_t init_once;
static pthread_t tid;

static void *thr_alrm(void *p) {
  while (1) {
    pthread_mutex_lock(&mut_job);
    for (int i = 0; i < MYTBF_MAX; i++) {
      if (job[i] != NULL) {
        pthread_mutex_lock(&job[i]->mut);
        job[i]->token += job[i]->cps;
        if (job[i]->token > job[i]->burst)
          job[i]->token = job[i]->burst;
        pthread_mutex_unlock(&job[i]->mut);
      }
    }
    pthread_mutex_unlock(&mut_job);
    sleep(1);
  }
}

static void moude_unload(void) {
  pthread_cancel(tid);
  pthread_join(tid, NULL);

  for (int i = 0; i < MYTBF_MAX; i++)
    free(job[i]);

  pthread_mutex_destroy(&mut_job);
}

static void moude_load(void) {
  int err;

  err = pthread_create(&tid, NULL, thr_alrm, NULL);
  if (err) {
    fprintf(stderr, "pthread_create():%s\n", strerror(errno));
    exit(1);
  }
  atexit(moude_unload);
}

static int get_free_pos_unlocked(void) {
  for (int i = 0; i < MYTBF_MAX; i++) {
    if (job[i] == NULL)
      return i;
  }
  return -1;
}

mytbf_t *mytbf_init(int cps, int rst) {
  struct mytbf_st *me;
  int pos;

  pthread_once(&init_once, moude_load);
  me = malloc(sizeof(*me));
  if (me == NULL) {
    return NULL;
  }
  me->token = 0;
  me->cps = cps;
  me->burst = cps;
  pthread_mutex_init(&me->mut, NULL);

  pthread_mutex_lock(&mut_job);
  pos = get_free_pos_unlocked();
  if (pos < 0) {
    pthread_mutex_unlock(&mut_job);
    return NULL;
  }

  me->pos = pos;
  job[pos] = me;

  pthread_mutex_unlock(&mut_job);

  return me;
}

int mytbf_fetchtoken(mytbf_t *ptr, int size) {
  struct mytbf_st *me = ptr;
  if (size <= 0)
    return -EINVAL;

  pthread_mutex_lock(&me->mut);
  while (me->token <= 0) {
    pthread_mutex_unlock(&me->mut);
    sched_yield();
    pthread_mutex_lock(&me->mut);
  }

  int n = me->token <= size ? me->token : size;
  me->token -= n;
  pthread_mutex_unlock(&me->mut);

  return n;
}

int mytbf_returntoken(mytbf_t *ptr, int size) {
  struct mytbf_st *me = ptr;

  if (size <= 0)
    return -EINVAL;

  pthread_mutex_lock(&me->mut);
  me->token += size;
  if (me->token > me->burst)
    me->token = me->burst;
  pthread_mutex_unlock(&me->mut);

  return size;
}

int mytbf_destrory(mytbf_t *ptr) {
  struct mytbf_st *me = ptr;

  pthread_mutex_lock(&mut_job);
  job[me->pos] = NULL;
  pthread_mutex_unlock(&mut_job);
  pthread_mutex_destroy(&me->mut);
  free(ptr);

  return 0;
}
