#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

#include "mysem.h"

#define THRNUM 200
#define N 5
#define LEFT 30000001
#define RIGHT 30000200

static mysem_t *sem;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static int num = 0;

static void *handler(void *p){
    int task,mark,count = 0;

    while(1){

        pthread_mutex_lock(&mutex);
        while(num == 0){
            pthread_cond_wait(&cond,&mutex);
        }
    
        if (num == -1){
            pthread_mutex_unlock(&mutex);
            break;
        }

        task = num;
        num = 0;//成功领取任务
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);

        mark = 1;
        for (int j = 2;j <= task/2;j++){
            if (task%j == 0){
                mark = 0;
                break;
            }
        }
        if (mark) {
            printf("[%d] %d is a priamer\n",*(int *)p,task);
        }
        count++;
        if (count == 1){
            sleep(1);
            break;
        }
    }

    //归还计算资源
    mysem_add(sem,1);

    pthread_exit(NULL);
}

//池类算法
int main()
{
    pthread_t Ptid[THRNUM];
    sem = mysem_init(N);//初始化计算资源

    for (int i = LEFT;i <= RIGHT;i++){
        mysem_sub(sem,1);//消耗一个计算资源
        int *ptid = malloc(sizeof(int));
        *ptid = i-LEFT;
        int err = pthread_create(Ptid+(i-LEFT),NULL,handler,ptid);
        if (err){
            fprintf(stderr,"%s\n",strerror(err));
            exit(1);
        }

        pthread_mutex_lock(&mutex);
        
        //任务没有被领取
        while(num != 0){
            pthread_cond_wait(&cond,&mutex);
        }
        //任务已经成功下发
        num = i;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    
    pthread_mutex_lock(&mutex);
    //任务没有被领取
    while(num != 0){
        pthread_cond_wait(&cond,&mutex);
    }
    //任务已经成功下发
    num = -1;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);

    int n;
    for (n =0 ;n < THRNUM;n++){
        pthread_join(Ptid[n],NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    exit(0);
}

