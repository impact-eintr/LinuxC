#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define N 24
#define core 8

static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;;
static int sum = 0;

static int arr[N] = {1,4,3,9,2,8,5,1,1,6,2,7,2,5,0,4,1,8,6,5,1,2,3,9};
static int res[core] = {0};
static int num = -1;

static void *handler(void *p){
    int my_x,my_sum = 0,my_task = 0;
    pthread_mutex_lock(&mutex);
    while(num == -1){
        pthread_cond_wait(&cond,&mutex);
    }
    my_task = num;
    num = -1;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);

    for (int i = my_task;i < my_task+(N/core);i++){
        my_x = arr[i];
        my_sum += my_x;
    }
    //sleep(1);
    res[my_task/(N/core)] = my_sum;
    printf("[%d]sum= %d\n",my_task,my_sum);

    pthread_exit(NULL);
}

int main()
{
    pthread_t ptid[N];

    for (int i = 0;i < core;i++){
        int err = pthread_create(ptid+i,NULL,handler,NULL);
        if (err) {
            fprintf(stderr,"%s\n",strerror(err));
            exit(1);
        }
    }

    for (int i = 0;i < N;i+=(N/core)){
        // 临界区
        pthread_mutex_lock(&mutex);

        while(num != -1){
            pthread_cond_wait(&cond,&mutex);
        }

        num = i;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }


    for (int n = 0;n < core;n++){
        pthread_join(ptid[n],NULL);
        sum += res[n];
    }

    printf("sum= %d\n",sum);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    exit(0);

}
