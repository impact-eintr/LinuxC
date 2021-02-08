#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

static volatile int loop = 1;

static void alarm_handler(int s)
{
    loop = 0;
}

int main()
{
    int64_t count = 0;

    alarm(5);
    signal(SIGALRM, alarm_handler);
    while (loop)
	count++;
    printf("%ld", count);
    exit(0);
}
