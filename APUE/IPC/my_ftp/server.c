#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <unistd.h>

#include "proto.h"


int main(){
    key_t key;
    int msgid;
    struct msg_st rbuf;

    key = ftok(KEYPATH,KEYPROJ);
    if (key < 0){
        perror("ftik()");
        exit(1);
    }
    
    msgid = msgget(key,IPC_CREAT|0666);
    if (msgid < 0){
        perror("msgget()");
        exit(1);
    }
    
    while(1){
        if (msgrcv(msgid,&rbuf,sizeof(rbuf)-sizeof(long),0,0) < 0){
            perror("msgrcv()");
            exit(1);
        }

        fprintf(stdout,"name : %s ",rbuf.name);
        fprintf(stdout,"chinese : %d ",rbuf.chinese);
        fprintf(stdout,"math : %d\n",rbuf.math);
        fflush(NULL);
    }
    //销毁当前实例
    msgctl(key,IPC_RMID,0);

    exit(0);
}
