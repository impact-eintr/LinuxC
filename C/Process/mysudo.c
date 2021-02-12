#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

int main(int argc,char **argv)
{
    if (argc < 3){
        fprintf(stderr,"Useage");
        exit(1);
    }

    pid_t pid;

    pid = fork();
    if (pid == 0){
        setuid(atoi(argv[1]));
        execvp(argv[2],argv+2);
        perror("execvp()");
        exit(1);

    }else {
        wait(NULL);
    }

    exit(0);
}

