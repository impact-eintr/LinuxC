#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define N 20

void handler(int sig){
    write(1,"!",1);
}


int main()
{
    int i;

    signal(SIGINT,handler);

    for (i = 0;i < N;i++){
        write(1,"*",1);
        sleep(1);
    }
    
    exit(0);
}
