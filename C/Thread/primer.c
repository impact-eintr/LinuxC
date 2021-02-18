#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

#define N 5
#define LEFT 30000000
#define RIGHT 30000200


static void *handler(void *p){
    int n = *(int *)p;
    int i,j,mark;
    for (i = LEFT+n;i <= RIGHT;i+=N){
        mark = 1;
        for (j = 2;j <= i/2;j++){
            if (i%j == 0){
                mark = 0;
                break;
            }
        }
        if (mark) {
            printf("%d is a primer [%d]\n",i,n);
        }
    }
    pthread_exit(p);

}

//交叉算法计算 池类算法涉及到竞争
int main()
{
    pthread_t Ptid[N];
    void *ptr = NULL;

    for (int n = 0;n < N;n++){
        int *num = malloc(sizeof(int));
        *num = n;
        int err = pthread_create(Ptid+n,NULL,handler,num);
        if (err){
            fprintf(stderr,"%s\n",strerror(err));
            exit(1);
        }
    }

    int n;
    for (n =0 ;n < N;n++){
        pthread_join(Ptid[n],ptr);
        free(ptr);
    }

    exit(0);
}

