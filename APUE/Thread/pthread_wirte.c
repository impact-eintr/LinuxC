#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define THRNUM 20
#define FNAME "/tmp/out"
#define BUFSIZE 1024

//互斥量
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void *handler(void *p){
    FILE *fp = fopen(FNAME,"r+");
    char buf[BUFSIZE];

    if(fp == NULL){
        perror("fopen()");
        exit(1);
    }

    //进入临界区
    pthread_mutex_lock(&mutex);
    fgets(buf,BUFSIZE,fp);
    fseek(fp,0,SEEK_SET);
    sleep(1);
    fprintf(fp,"%d\n",atoi(buf)+1);
    fclose(fp);
    pthread_mutex_unlock(&mutex);
    //离开临界区

    pthread_exit(NULL);
}

int main()
{
    pthread_t Ptid[THRNUM];

    for (int i = 0;i < THRNUM;i++){
        int err = pthread_create(Ptid+i,NULL,handler,NULL);
        if (err){
           fprintf(stderr,"%s\n",strerror(err));
           exit(1);
        }
    }

    for (int i = 0;i < THRNUM;i++){
        pthread_join(Ptid[i],NULL);
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}

