#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

#define THRNUM 50
#define LEFT 30000001
#define RIGHT 30000200

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;;
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
        if (count == 5){
            break;
        }
    }

    pthread_exit(NULL);
}

//池类算法
int main()
{
    pthread_t Ptid[THRNUM];

    for (int n = 0;n < THRNUM;n++){
        int *num = malloc(sizeof(int));
        *num = n;
        int err = pthread_create(Ptid+n,NULL,handler,num);
        if (err){
            fprintf(stderr,"%s\n",strerror(err));
            exit(1);
        }
    }

    for (int i = LEFT;i <= RIGHT;i++){
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

