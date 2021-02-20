#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

static void *func(void *p){
    puts("thread is working");
    pthread_exit(NULL);
}

int main()
{
    printf("Begin\n");
    pthread_t ptid;

    int err = pthread_create(&ptid,NULL,func,NULL);
    if (err){
        fprintf(stderr,"pthread_create err %s\n",strerror(err));
        exit(1);
    }
    
    pthread_join(ptid,NULL);
    printf("End\n");
    exit(0);
}

