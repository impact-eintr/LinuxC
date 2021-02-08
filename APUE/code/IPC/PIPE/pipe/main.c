#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFSIZE 1024

int main()
{
    int pd[2];
    int len;
    char buf[BUFSIZE];
    pid_t pid;

    if (pipe(pd) < 0) {
        perror("pipe()");
        exit(1);
    }

    pid = fork();
    if (pid < 0) {
        perror("fork()");
        exit(1);
    }

    if (pid == 0) {
        close(pd[1]);                    //关读端
        len = read(pd[0], buf, BUFSIZE); //开写端
        write(1, buf, len);
        close(pd[0]);
        exit(0);
    } else {
        close(pd[0]);
        write(pd[1], "Hello~\n", 7);
        close(pd[1]);
        wait(NULL);
        exit(0);
    }

    exit(0);
}
