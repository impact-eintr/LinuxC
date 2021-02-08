#include "proto.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>

int main()
{
    key_t key;
    struct msg_st sbuf;
    int msgid;

    key = ftok(KEYPATH, KEYPROJ);
    if (key < 0) {
        perror("ftok()");
        exit(1);
    }
    //创建接收方
    msgid = msgget(key, 0);
    if (msgid < 0) {
        perror("msgget()");
        exit(1);
    }
    //处理接收的数据
    sbuf.mytype = 1;
    strcpy(sbuf.name, "Mike"); //name 是数组名 常量不能直接赋值
    sbuf.math = rand() % 100;
    sbuf.chinese = rand() % 100;
    if (msgsnd(msgid, &sbuf, sizeof(sbuf) - sizeof(long), 0) < 0) {
        perror("msgrcv()");
        exit(1);
    }

    printf("200\n");
    exit(0);
}
