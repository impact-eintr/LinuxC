#include "mysem.h"
#include <stdio.h>
#include <stdlib.h>

#define LEFT 30000000
#define RIGHT 30000200
#define THRNUM RIGHT - LEFT
#define N 4

static mysem_t *sem;

static void *thr_primer(void *p) {
  int i = (int)p;
  int flag = 1;
  for (int j = 2; j < i / 2; j++) {
    if (i % j == 0) {
      flag = 0;
      break;
    }
  }
  if (flag)
    printf("%d is a primer\n", i);

  sleep(3);
  mysem_add(sem, 1);
  pthread_exit(p);
}

int main(void) {
  int err;
  pthread_t tid[THRNUM];

  sem = mysem_init(N);
  if (sem == NULL) {
    fprintf(stderr, "mysem_init faild!\n");
    exit(1);
  }

  for (int i = LEFT; i <= RIGHT; i++) {
    mysem_sub(sem, 1);
    err = pthread_create(tid + (i - LEFT), NULL, thr_primer, (void *)i);
    if (err) {
      fprintf(stderr, "pthread_create(:%s\n)", strerror(err));
    }
  }

  for (int i = LEFT; i <= RIGHT; i++) {
    pthread_join(tid[i - LEFT], NULL);
  }

  mysem_destroy(sem);
  exit(0);
}
