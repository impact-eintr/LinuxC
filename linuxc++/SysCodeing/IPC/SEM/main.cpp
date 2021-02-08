#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
void msg_stat(int ,struct msqid_ds);

int main(){
    int gflags,sflags,rflags;
    key_t key;
    int msgid;
    int reval;
    struct msgsbuf{
        int mytype;
        char mtext[1];
    }msg_sbuf;

    struct msgmbuf{
        int mytype;
        char mtext[10];
    }msg_rbuf;
    struct msqid_ds msg_ginfo,msg_sinfo;
    char msgpath[] = "./test";

    key = ftok(msgpath,233);
    gflags = IPC_CREAT|IPC_EXCL;
    msgid = msgget(key,gflags|0666);
    if(msgid == -1){
        perror("msgget()");
        exit(1);
    }
    msg_stat(msgid,msg_ginfo);
    sflags = IPC_NOWAIT;
    msg_sbuf.mytype = 10;
    msg_sbuf.mtext[0] = 'a';
    reval = msgctl(msgid,IPC_RMID,NULL);
    if(reval == -1){
        printf("unlink msg queue error\n");
    }
}
