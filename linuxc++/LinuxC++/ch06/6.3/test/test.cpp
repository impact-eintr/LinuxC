#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void handler(int sig) {
    printf("Deal SIGINT"); // SIGINT信号处理函数
}
int main() {
    sigset_t newmask;
    sigset_t oldmask;
    sigset_t pendmask;

    struct sigaction act;
    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, 0);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGINT);
    sigprocmask(SIG_BLOCK, &newmask, &oldmask);
    sleep(5);
    sigpending(&pendmask);

    if (sigismember(&pendmask, SIGINT))
      printf(" SIGINT pending\n");

    sigprocmask(SIG_SETMASK, &oldmask, NULL);

    printf("SIGINT unblocked\n");
    sleep(5);
    return (0);
}
