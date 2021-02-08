#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

void *handler1(void *arg){
    printf("in handler1\n");
    return (void *) 0;
}

void *handler2(void *arg){
    int *pn = (int *)(arg);
    int n = *pn;

    printf("in handler2 : n = %d\n",n);
    return (void *) 0;
}

void *handler3(void *arg){
    char *p = (char *)arg;
    printf("in handler3 : str=%s\n", p); //打印结构体的内容
    return (void *)0;
}

typedef struct{
    int n;
    const char *str;
}MYSTRUCT;

void *handler4(void *arg){
    MYSTRUCT *p = (MYSTRUCT *)arg;
    printf("in handler4 : n := %d,str = %s\n",p->n,p->str);
    return (void *)0;
}

int main(){
    pthread_t tidp[4];//threadidptr

    int ret1 = pthread_create(&*(tidp+0),NULL,handler1,NULL);
    if(ret1){
        perror("pthread_create()");
        exit(1);
    }
    pthread_join(*(tidp), NULL); //等待子线程结束
    printf("in main:thread1 is created\n");

    //传参版本
    int n = 233;
    int ret2 = pthread_create(&*(tidp+1),NULL,handler2,&n);
    if(ret2){
        perror("pthread_create()");
        exit(1);
    }
    pthread_join(*(tidp+1), NULL); //等待子线程结束
    printf("in main:thread2 is created\n");

    int ret3;
    const char *str = "hello world";

    ret3 = pthread_create(&*(tidp+2), NULL, handler3, (void *)str);//创建线程并传递结构体地址
    if (ret3)
    {
        printf("pthread_create failed:%d\n", ret3);
        return -1;

    }
    pthread_join(*(tidp+2), NULL); //等待子线程结束
    printf("in main:thread3 is created\n");

    MYSTRUCT me;
    me.n = 666;
    me.str = "你好";

    int ret4 = pthread_create(&*(tidp+3), NULL, handler4, (void *)&me);//创建线程并传递结构体地址
    if (ret4)
    {
        printf("pthread_create failed:%d\n", ret3);
        return -1;

    }
    pthread_join(*(tidp+3), NULL); //等待子线程结束
    printf("in main:thread4 is created\n");

    exit(0);
}
