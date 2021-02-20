#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/sem.h>

#define THRNUM 20
#define FNAME "/tmp/out"
#define BUFSIZE 1024


static void P(){

}

static void V(){

}

static void handler(){
    FILE *fp = fopen(FNAME,"r+");
    char buf[BUFSIZE];


    P();

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

    V();

    //离开临界区
    lockf(fd,F_ULOCK,0);

    fclose(fp);
}

int main()
{
    pid_t pid;
    int semid;

    semid = semget(IPC_PRIVATE,1,0666);
    if (semid < 0){
        perror("semget()");
        exit(1);
    }
    //初始化
    if (semctl(semid,0,SETVAL,1)){//相当于互斥量
        perror("semctl()");
        exit(1);

    }    

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
    semctl(semid,0,IPC_RMID);

    return 0;
}

