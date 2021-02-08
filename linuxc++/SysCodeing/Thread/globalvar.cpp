#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

static int N = 100;

void *handler(void *arg){
    N++;
    printf("in handler N = %d\n",N);
    return (void *)0;
}

int main(){
    pthread_t tidp;
    int ret;

    ret = pthread_create(&tidp,NULL,handler,NULL);
    if(ret){
        perror("pthread_create()");
        exit(1);
    }
    pthread_join(tidp,NULL);

    N++;
    printf("in main N = %d\n",N);
    exit(0);

}
