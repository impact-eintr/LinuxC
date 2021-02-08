#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void int_handler(int s)
{
    write(1, "?", 1);
}

int main()
{
    sigset_t set, o_set, save_set;
    signal(SIGINT, int_handler);
    sigemptyset(&set);
    sigaddset(&set, SIGINT);

    sigprocmask(SIG_UNBLOCK, &set, &save_set);

    for (int i = 0; i < 1000; i++) {
	sigprocmask(SIG_BLOCK, &set, &o_set);
	for (int j = 0; j < 5; j++) {
	    write(1, "*", 1);
	    sleep(1);
	}
	write(1, "\n", 1);
	sigprocmask(SIG_SETMASK, &o_set, NULL);
    }
    sigprocmask(SIG_SETMASK, &save_set, NULL);
    exit(0);
}
