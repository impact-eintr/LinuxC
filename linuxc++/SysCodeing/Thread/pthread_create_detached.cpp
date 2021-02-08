#include <iostream>
#include <pthread.h>
#include <unistd.h>

void *thfunc(void *arg){
    sleep(1);
    std::cout << ("sub thread is running\n");
    return NULL;
}

int main(){
    pthread_t thread_id;
    pthread_attr_t thread_attr;
    struct sched_param thread_patam;
    size_t stack_size;
    int res;

    res = pthread_attr_init(&thread_attr);
    if(res)
      std::cout << "Pthread_attr_init failed:"<<res<<std::endl;

    res = pthread_attr_setdetachstate(&thread_attr,PTHREAD_CREATE_DETACHED);
    if(res)
      std::cout << "Pthread set  failed:"<<res<<std::endl;
    res = pthread_create(&thread_id,&thread_attr,thfunc,NULL);
    if(res)
      std::cout << "pthread_create failed:\n"<<std::endl;
    //pthread_join(thread_id,NULL);
    pthread_exit(NULL);
    std::cout << "main thread will exit\n" << std::endl;//主线程退出,但进程不会退出
    exit(0);
}
