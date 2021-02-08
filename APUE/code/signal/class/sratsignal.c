#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void int_handler(int sig)
{
    write(1, "!", 1);
}

int main()
{

    signal(SIGINT, int_handler);
    for (int i = 0; i < 5; i++) {
	write(1, "*", 1);
	fflush(NULL);
	sleep(1);
    }
    exit(0);
}
