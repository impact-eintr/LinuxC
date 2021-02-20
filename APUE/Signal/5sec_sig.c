#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

//使用 信号 计算当前单核 累加 的速度

static volatile int loop = 1;

static void handler(int sig){
    loop = 0;
}

int main()
{
    int64_t count = 0;

    alarm(5);
    signal(SIGALRM,handler);

    while(loop){
        count++;
    }

    printf("count %ld",count);

    exit(0);
}

