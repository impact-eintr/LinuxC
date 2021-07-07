#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

#define N 3
#define LEFT 100000002
#define RIGHT 100000200

//交叉算法计算 池类算法涉及到竞争
int main()
{
    printf("[%d] start !\n",getpid());
    fflush(NULL);//记得刷新 否则begin放到缓冲区 父子进程的缓冲区里各有一句begin
    pid_t pid = 0;
    int i,j,mark;

    for (int n = 0;n < N;n++){
        pid = fork();
        if (pid < 0){
            perror("fork");
            for (int k = 0;k < n;k++){
                wait(NULL);
            }
            exit(1);
        }

        if (pid == 0){
            for (i = LEFT+n;i <= RIGHT;i+=N){
                mark = 1;
                for (j = 2;j <= i/2;j++){
                    if (i%j == 0){
                        mark = 0;
                        break;
                    }
                }
                if (mark) {
                    printf("%d is a primer\n",i);
                }
            }
            printf("[%d] exit\n",n);
            exit(0);
        }

    }

    int st,n;
    for (n =0 ;n < N;n++){
        wait(&st);
        printf("%d end\n",st);
    }

    exit(0);
}

