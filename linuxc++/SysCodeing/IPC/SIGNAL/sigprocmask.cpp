#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <iostream>
#include <unistd.h>

void handler(int s){
    std::cout << "\nDealing SIGINT\n"<<std::endl;
}

int main(){
    sigset_t newmask;
    sigset_t oldmask;
    sigset_t pendmask;

    struct sigaction act;
    act.sa_handler = handler;
    act.sa_flags = 0;
    sigaction(SIGINT, &act, 0);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGINT);
    sigprocmask(SIG_BLOCK, &newmask, &oldmask);
    sleep(3);
    sigpending(&pendmask);
    //用于检查是否有挂起的信号

    if (sigismember(&pendmask, SIGINT))//判断被挂起的信号是否是SIGINT
      printf(" SIGINT pending\n");

    sigprocmask(SIG_SETMASK, &oldmask, NULL);

    printf("SIGINT unblocked\n");
    sleep(100);
    return (0);

}
