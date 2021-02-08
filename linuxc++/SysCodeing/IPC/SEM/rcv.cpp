#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#define MAX_TEXT 512

struct my_msg_st{
    long int my_msg_type;
    char some_text[MAX_TEXT];
};

int main(){
    int running = 1;
    struct my_msg_st some_data;
    int msgid;
    char buffer[BUFSIZ];

    msgid = msgget((key_t)1234,0666|IPC_CREAT);
    if(msgid == -1){
        perror("msgget()");
        exit(1);
    }
    while(running){
        std::cout << "Enter :";
        std::cin>>buffer;
        //std::cin.ignore();
        //fgets(buffer,BUFSIZ,stdin);
        some_data.my_msg_type = 1;
        strcpy(some_data.some_text,buffer);
        if(msgsnd(msgid,(void *)&some_data,MAX_TEXT,0) == -1){
            fprintf(stderr,"msgsnd faild\n");
            exit(1);
        }
        if(strncmp(buffer,"end",3) == 0){
            running = 0;
        }
    }
    exit(0);
}
