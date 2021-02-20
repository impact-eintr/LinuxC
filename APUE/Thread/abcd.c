#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

#define N 4
#define LEFT 30000000
#define RIGHT 30000200

static pthread_mutex_t mutex_arr[N];
//static pthread_cond_t cond;

static void *handler(void *p){
    int n = *(int *)p;
    int ch = '1'+n;

    while(1){
        pthread_mutex_lock(mutex_arr+n);
        write(1,&ch,1);
        pthread_mutex_unlock(mutex_arr+(n+1)%N);
    }
    pthread_exit(p);
}

int main()
{
    pthread_t Ptid[N];
    void *ptr = NULL;

    for (int n = 0;n < N;n++){
        int *num = malloc(sizeof(int));
        *num = n;

        pthread_mutex_init(mutex_arr+n,NULL);
        pthread_mutex_lock(mutex_arr+n);

        int err = pthread_create(Ptid+n,NULL,handler,num);
        if (err){
            fprintf(stderr,"%s\n",strerror(err));
            exit(1);
        }
    }

    pthread_mutex_unlock(mutex_arr+0);
    alarm(3) ;   

    int n;
    for (n =0 ;n < N;n++){
        pthread_join(Ptid[n],ptr);
        free(ptr);
    }

    exit(0);
}

