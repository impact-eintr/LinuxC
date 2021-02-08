#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    puts("Start!");

    pid_t pid;
    pid = fork();
    if (pid < 0) {
	perror("fork()");
	exit(1);
    }
    if (pid == 0) {
	execl("/usr/bin/date", "date", "+%s", NULL);
	perror("execl()");
	exit(1);
    }

    wait(NULL);

    puts("Ends");
    exit(0);
}
