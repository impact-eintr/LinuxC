#include <cstdlib>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t childpid;
    int status;
    int retval;

    childpid = fork();
    if (-1 == childpid) {
        perror("fork()");
        exit(EXIT_FAILURE);
    } else if (childpid == 0) {
        puts("在子进程中...");
        sleep(100);
        exit(EXIT_SUCCESS);
    } else {
        sleep(1);
        if (waitpid(childpid, &status, WNOHANG) == 0) {
            puts("有子进程在跑...");
            retval = kill(childpid, SIGKILL);

            if (retval) {
                puts("kill faild.");
                perror("kill()");
                waitpid(childpid, &status, 0);
            }else{
                printf("%d killed\n",childpid);
            }
        }

    }
}
