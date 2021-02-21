#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/shm.h>

#define BUFSIZE 1024
#define SHMSIZE 4*1024*1024

static int shmid;

int main()
{
    pid_t pid;
    char *ptr;

    shmid = shmget(IPC_PRIVATE,SHMSIZE,0666);
    if (shmid < 0){
        perror("shmget()");
        exit(1);
    }

    pid = fork() ;
    if (pid < 0){
        perror("fork()");
        exit(1);
    }
    if (pid == 0){
        ptr = shmat(shmid,NULL,0);
        if (ptr == (void *)-1){
            perror("shmat()");
            exit(1);
        }
        strcpy(ptr,"hello");
        shmdt(ptr);
        exit(0);
    }else{
        wait(NULL);
        ptr = shmat(shmid,NULL,0);
        if (ptr == (void *)-1){
            perror("shmat()");
            exit(1);
        }
        puts(ptr);
        shmdt(ptr);
        shmctl(shmid,IPC_RMID,NULL);
        exit(0);

    }
}

