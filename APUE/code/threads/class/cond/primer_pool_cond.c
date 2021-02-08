#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LEFT 30000000
#define RIGHT 30000200
#define THRNUM 4

static int num = 0;
static pthread_mutex_t mut_num = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond_num = PTHREAD_COND_INITIALIZER;

static void* thr_primer(void* p)
{
    int i, flag;
    pthread_mutex_lock(&mut_num);
    while (1) {
	pthread_mutex_unlock(&mut_num);
	while (num == 0) {
	    pthread_cond_wait(&cond_num, &mut_num);
	}

	if (num == -1) {
	    pthread_mutex_unlock(&mut_num);
	    break;
	}

	i = num;
	num = 0;
	pthread_cond_broadcast(&cond_num);
	pthread_mutex_unlock(&mut_num); //释放num

	flag = 1;
	for (int j = 2; j < i / 2; j++) {
	    if (i % j == 0) {
		flag = 0;
		break;
	    }
	}
	if (flag)
	    printf("%d is a primer\n", i);
    }
    pthread_exit(p);
}

int main(void)
{
    int err;
    pthread_t tid[THRNUM];
    for (int i = 0; i < THRNUM; i++) {
	err = pthread_create(tid + i, NULL, thr_primer, (void*)i);
	if (err) {
	    fprintf(stderr, "pthread_create(:%s\n)", strerror(err));
	    exit(1);
	}
    }

    //下发任务
    for (int i = LEFT; i <= RIGHT; i++) {
	pthread_mutex_lock(&mut_num);
	while (num != 0) {
	    pthread_cond_wait(&cond_num, &mut_num);
	}
	num = i; //下发任务
	pthread_cond_signal(&cond_num);
	pthread_mutex_unlock(&mut_num);
    }

    //检查是否结束
    pthread_mutex_lock(&mut_num);
    while (num != 0) {
	pthread_cond_wait(&cond_num, &mut_num);
    }
    num = -1; //结束线程
    pthread_cond_broadcast(&cond_num);
    pthread_mutex_unlock(&mut_num);

    for (int i = 0; i < THRNUM; i++) {
	pthread_join(tid[i], NULL);
    }

    pthread_mutex_destroy(&mut_num);
    pthread_cond_destroy(&cond_num);
    exit(0);
}
