#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>

#define THRNUM 20
#define FNAME "/tmp/out"
#define BUFSIZE 1024

static void handler(){
    FILE *fp = fopen(FNAME,"r+");
    char buf[BUFSIZE];

    if(fp == NULL){
        perror("fopen()");
        exit(1);
    }
    int fd = fileno(fp);
    //进入临界区
    lockf(fd,F_LOCK,0);

    fgets(buf,BUFSIZE,fp);
    fseek(fp,0,SEEK_SET);
    sleep(1);
    fprintf(fp,"%d\n",atoi(buf)+1);
    fflush(fp);

    //离开临界区
    lockf(fd,F_ULOCK,0);

    fclose(fp);
}

int main()
{
    pid_t pid;

    for (int i = 0;i < THRNUM;i++){
        pid = fork() ;
        if (pid < 0){
            perror("fork()");
            exit(1);
        }
        if (pid == 0){
            handler();
            exit(0);
        }
    }

    for (int i = 0;i < THRNUM;i++){
        wait(NULL);
    }

    return 0;
}

