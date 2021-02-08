#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define LEFT 20000000
#define RIGHT 20000500
int main(void)
{
    int flag = 0;
    pid_t pid;

    for (int i = LEFT; i <= RIGHT; i++) {
	pid = fork();
	if (pid == 0) {
	    flag = 1;
	    for (int j = 2; j < i / 2; j++) {
		if (i % j == 0) {
		    flag = 0;
		    break;
		}
	    }
	    if (flag)
		printf("%d is a primer\n", i);
	    exit(0);
	}
    }
    sleep(1000);
    exit(0);
}
