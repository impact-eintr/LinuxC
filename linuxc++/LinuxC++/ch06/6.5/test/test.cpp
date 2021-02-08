#include <stdio.h>
#include <signal.h>
typedef void(*signal_handler)(int);

void signal_handler_fun(int signum) {
	printf("catch signal %d\n", signum);
}

int main(int argc, char *argv[]) {
	signal(SIGINT, signal_handler_fun);
	while (1)
		;
	return 0;
}