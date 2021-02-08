#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    pid_t pid;
    printf("[%d] parent begins\n", getpid());

    fflush(NULL);
    pid = fork();

    if (pid == 0) {
	printf("[%d] child begins\n", getpid());
    } else {
	printf("[%d] parent working\n", getpid());
    }
    printf("[%d]process  ends;\n", getpid());
    getchar();
    exit(0);
}
