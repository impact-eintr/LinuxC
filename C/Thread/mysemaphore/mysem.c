#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

#include "mysem.h"

struct mysem_st{
    int vaclue;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

mysem_t *mysem_init(int initval){
    struct mysem_st *sem;
    sem = malloc(sizeof(*sem));

    if (sem == NULL){
        return NULL;
    }
    
    sem->vaclue = initval;
    pthread_mutex_init(&sem->mutex,NULL);
    pthread_cond_init(&sem->cond,NULL);

    return sem;
}

int mysem_add(mysem_t *ptr,int n){

    struct mysem_st *sem = ptr;

    pthread_mutex_lock(&sem->mutex);
    sem->vaclue += n;
    pthread_cond_broadcast(&sem->cond);
    pthread_mutex_unlock(&sem->mutex);

    return n;
}

int mysem_sub(mysem_t *ptr,int n){
    struct mysem_st *sem = ptr;

    pthread_mutex_lock(&sem->mutex);

    while(sem->vaclue < n){
        pthread_cond_wait(&sem->cond,&sem->mutex);
    }
    sem->vaclue -= n;
    pthread_mutex_unlock(&sem->mutex);
    
    return n;
}

int mysem_destory(mysem_t *ptr){
    struct mysem_st *sem = ptr;

    pthread_mutex_destroy(&sem->mutex);
    pthread_cond_destroy(&sem->cond);
    free(sem);

    return 0;
}

