#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

static void cleanup(void *p){
    puts(p);
}

static void *func(void *p){
    puts("Thread is working");

    pthread_cleanup_push(cleanup,"1");//这个函数是宏
    pthread_cleanup_push(cleanup,"2");
    pthread_cleanup_push(cleanup,"3");
    
    //pthread_exit(NULL);
    //下面的内容执行不到但是不会报错 会按照全为 1 处理

    pthread_cleanup_pop(1);//语法结构一定要对等
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(1);

    puts("push over");

    pthread_exit(NULL);
}

int main()
{
    pthread_t ptid;
    printf("Start !\n");

    int err = pthread_create(&ptid,NULL,func,NULL);//ptid 属性 处理函数 传参
    if (err){
        fprintf(stderr,"pthread_create() : %s\n",strerror(err));
        exit(1);
    }

    pthread_join(ptid,NULL);

    printf("End !\n");
    exit(0);
}

