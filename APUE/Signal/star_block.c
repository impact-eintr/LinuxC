#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define N 5

void handler(int sig){
    write(1,"S",1);
}


int main()
{
    int i;

    sigset_t sigset,old_sigset,sigset_status;
    sigemptyset(&sigset);
    sigaddset(&sigset,SIGINT);

    signal(SIGINT,handler);
    
    //保存进入该模块前的状态
    sigprocmask(SIG_UNBLOCK,&sigset_status,NULL);
    sigprocmask(SIG_BLOCK,&sigset,&old_sigset);
    while(1){
        for (i = 0;i < N;i++){
            write(1,"*",1);
            sleep(1);
        }
        write(1,"\n",1);
        sigsuspend(&old_sigset);
    }
    //恢复进入该模块前的状态
    sigprocmask(SIG_SETMASK,&sigset_status,NULL);

    exit(0);
}
