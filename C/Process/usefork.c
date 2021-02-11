#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define LEFT 2
#define RIGHT 200

int main()
{
    printf("[%d] start !\n",getpid());
    fflush(NULL);//记得刷新 否则begin放到缓冲区 父子进程的缓冲区里各有一句begin
    pid_t pid = 0;
    int i,j,mark;

    for (i = LEFT;i <= RIGHT;i++){
        pid = fork();
        if (pid == 0){
            mark = 1;
            for (j = 2;j < i/2;j++){
                if (i%j == 0){
                    mark = 0;
                    break;
                }
            }
            if (mark) {
                printf("%d is a primer\n",i);
            }
            exit(0);
        }
    }
    getchar();

    exit(0);
}

