#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define THRNUM 5
#define LINESIZE 2
#define FNAME "./log"

static int semid;
//申请资源
static void P(void)
{
    struct sembuf op;

    op.sem_num = 0;
    op.sem_op = -1;
    op.sem_flg = 0;

    if (semop(semid, &op, 1) < 0) {
        if (errno != EINTR || errno != EAGAIN) {
            perror("semop");
            exit(1);
        }
    }
}

//归还资源
static void V(void)
{
    struct sembuf op;

    op.sem_num = 0;
    op.sem_op = 1;
    op.sem_flg = 0;

    if (semop(semid, &op, 1) < 0) {
        if (errno != EINTR || errno != EAGAIN) {
            perror("semop");
            exit(1);
        }
    }
}

static void thr_add(void)
{
    FILE* fp;
    char linebuf[LINESIZE];

    fp = fopen(FNAME, "r+");

    if (fp == NULL) {
        perror("fopen()");
        exit(1);
    }

    P();
    fgets(linebuf, LINESIZE, fp);
    fseek(fp, 0, SEEK_SET);
    //sleep(1);
    fprintf(fp, "%d", atoi(linebuf) + 1);
    printf("%d\n", atoi(linebuf) + 1);
    fflush(fp);
    V();
    fclose(fp);
}

int main()
{
    int pid;

    semid = semget(IPC_PRIVATE, 2, 0600);
    if (semid < 0) {
        perror("semget()");
        exit(1);
    }

    if (semctl(semid, 0, SETVAL, 1) < 0) {
        perror("semctl()");
        exit(0);
    }

    for (int i = 0; i < THRNUM; i++) {
        pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(1);
        }
        if (pid == 0) {
            thr_add();
            exit(0);
        }
    }

    for (int i = 0; i < THRNUM; i++) {
        wait(NULL);
    }

    semctl(semid, 0, IPC_RMID);
    exit(0);
}
