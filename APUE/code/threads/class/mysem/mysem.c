#include "mysem.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
struct mysem_st {
  int value;
  pthread_mutex_t mut;
  pthread_cond_t cond;
};
mysem_t *mysem_init(int initval) {
  struct mysem_st *me;
  me = malloc(sizeof(*me));
  if (me == NULL)
    return NULL;

  me->value = initval;
  pthread_mutex_init(&me->mut, NULL);
  pthread_cond_init(&me->cond, NULL);
  return me;
}

//还资源量
int mysem_add(mysem_t *ptr, int n) {
  struct mysem_st *me = ptr;

  pthread_mutex_lock(&me->mut);
  me->value += n;
  pthread_cond_broadcast(&me->cond);
  pthread_mutex_unlock(&me->mut);

  return n;
}
//申请资源量
int mysem_sub(mysem_t *ptr, int n) {
  struct mysem_st *me = ptr;

  pthread_mutex_lock(&me->mut);

  while (me->value < n)
    pthread_cond_wait(&me->cond, &me->mut);

  me->value -= n;
  pthread_mutex_unlock(&me->mut);

  return n;
}

int mysem_destroy(mysem_t *ptr) {
  struct mysem_st *me = ptr;
  pthread_mutex_destroy(&me->mut);
  pthread_cond_destroy(&me->cond);
  free(me);

  return 0;
}
