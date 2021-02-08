#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#define LEFT 20000000
#define RIGHT 20000500
#define N 3

int main(void)
{
    int flag = 0;
    pid_t pid;

    for (int n = 0; n < N; n++) {
	pid = fork();
	if (pid == 0) {
	    for (int i = LEFT + n; i <= RIGHT; i += N) {
		flag = 1;
		for (int j = 2; j < i / 2; j++) {
		    if (i % j == 0) {
			flag = 0;
			break;
		    }
		}
		if (flag)
		    printf("[%d]%d is a primer\n", n, i);
	    }
	    exit(0);
	}
    }
    for (int i = 0; i <= N; i++)
	wait(NULL);

    exit(0);
}
