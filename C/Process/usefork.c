#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define LEFT 300000000
#define RIGHT 300002000

static void prim(){


    for (int i = LEFT;i <= RIGHT;i++){
        int mark = 1;
        for (int j = 2;j < i/2;j++){
            if (i%j == 0){
                mark = 0;
                break;
            }
        }
        if (mark) {
            printf("%d is a primer\n",i);
        }
    }
}

int main()
{
    printf("[%d] start !\n",getpid());
    fflush(NULL);//记得刷新 否则begin放到缓冲区 父子进程的缓冲区里各有一句begin

    pid_t pid = fork();

    if (pid == 0){
        printf("child %d\n",getpid());
        prim();
    }else{
        printf("parent %d\n",getpid());
        prim();
    }

    printf("pid %d end\n",getpid());

    exit(0);
}

