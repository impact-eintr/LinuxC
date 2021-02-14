#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

int main()
{
    alarm(5);
    while(1){
        pause();
    }

    exit(0);
}

