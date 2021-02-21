# 单机IPC(进程间通信) 
IPC Inter-Process Communication
## 管道
- **由linux内核创建与维护**
- **单工 一端读 一端写**
- **自同步机制 流速快的 等 流速慢的**
#### 匿名管道
**用于有血缘关系的进程间通信**
- pipe()

形同 `cat ./test.mp3 | map123 -`

~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <wait.h>
#include <errno.h>

#define BUFSIZE 1024

int main()
{
    int pd[2];
    pid_t pid;
    char buf[BUFSIZE];

    if (pipe(pd) < 0){
        perror("pipe()");
        exit(1);
    }

    fprintf(stdout,"hello,welcome to MusicPlayer\n");

    pid = fork();
    if (pid < 0){
        perror("fork()");
        exit(1);
    }
    
    if(pid == 0){
        close(pd[1]); //关闭写端

        dup2(pd[0],0);//将将读端重定向到stdin mpg123 - "-" 表示从stdin读取
        close(pd[0]);

        int fd = open("/dev/null",O_RDWR);//屏蔽其他标准输出
        dup2(fd,1);
        dup2(fd,2);

        execl("/bin/mpg123","mpg123","-",NULL);
        perror("execvp()");
        exit(1);
    }else{
        close(pd[0]);//关闭读端
        int fd = open("./test.mp3",O_RDONLY);
        int len;

        while(1){
            len = read(fd,buf,BUFSIZE);
            if (len < 0){
                if (errno == EINTR)
                  continue;
                close(pd[1]);
                exit(1);
            }
            if (len == 0){
                break;
            }
            write(pd[1],buf,len);
        }
        close(pd[1]);
        wait(NULL);
    }

    exit(0);
}

~~~

#### 命名管道
- mkfifo 
~~~ bash
makefifo musicpipe
cat ./test.mp3 > musicpipe
~~~

~~~ bash
#同一个目录
mpg123 musicpipe
~~~

## XSI(SysV)
- `ipcs`命令
- ftok() **使得通信双方拿到同一个机制**
- 函数的命名方式类似
    - xxxget 获取
    - xxxop 操作
    - xxxctl 控制
### 消息队列 Message Queues
- msgget()
- msgop()
- msgctl()

~~~ c
#ifndef PROTO_H__
#define PROTO_H__

#define KEYPATH "/etc/services"
#define KEYPROJ 'g'
#define NAMESIZE 1024

struct msg_st{
    long mtype;
    char name[NAMESIZE];
    int math;
    int chinese;
};


#endif

~~~

~~~ c
#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include<sys/types.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>

#include "proto.h"


int main(){
    
    key_t key;
    int msgid;
    struct msg_st sbuf;

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
    
    strcpy(sbuf.name,"EINTR");
    sbuf.chinese = 100;
    sbuf.math = 100;
    
    if (msgsnd(msgid,&sbuf,sizeof(sbuf)-sizeof(long),0) < 0){
        perror("msgrcv()");
        exit(1);
    }

    fprintf(stdout,"OK");

    //销毁当前实例
    msgctl(key,IPC_RMID,0);

    exit(0);
}
~~~

~~~ c
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
~~~
        
### 信号量数组 Semaphore Arrays
- semget()
- semop()
- semctl()
~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/sem.h>

#define THRNUM 20
#define FNAME "/tmp/out"
#define BUFSIZE 1024

static int semid;

//取资源量
static void P(){
    struct sembuf op;

    op.sem_num = 0;
    op.sem_op = -1;//取一个资源
    op.sem_flg = 0;//特殊要求

    while(semop(semid,&op,1) < 0){
        if (errno == EINTR||errno == EAGAIN){
            continue;
        }else{
            perror("semop()");
            exit(1);
        }
    }

}

//还资源量
static void V(){
    struct sembuf op;

    op.sem_num = 0;
    op.sem_op = 1;//取一个资源
    op.sem_flg = 0;//特殊要求

    while(semop(semid,&op,1) < 0){
        if (errno == EINTR||errno == EAGAIN){
            continue;
        }else{
            perror("semop()");
            exit(1);
        }
    }

}

static void handler(){
    FILE *fp = fopen(FNAME,"r+");
    char buf[BUFSIZE];


    if(fp == NULL){
        perror("fopen()");
        exit(1);
    }
    int fd = fileno(fp);

    //进入临界区
    P();

    fgets(buf,BUFSIZE,fp);
    fseek(fp,0,SEEK_SET);
    sleep(1);
    fprintf(fp,"%d\n",atoi(buf)+1);
    fflush(fp);

    //离开临界区
    V();

    fclose(fp);
}

int main()
{
    pid_t pid;

    semid = semget(IPC_PRIVATE,1,0666);//父子关系的进程通信可以使用匿名IPC
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

~~~
### 共享内存 Shared Memory
- shmget()
- shmop()
- shmctl()

~~~ c
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

~~~
