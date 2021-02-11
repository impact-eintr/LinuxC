# 进程


### PID
类型 pid_t
- ps axf 查看进程树
- ps axm
- ps ax -L
- ps -ef
### 进程的产生
- fork() 
        - 注意理解关键字 duplicating 意味着拷贝 克隆 一模一样
        - fork 后父子进程的区别 ： fork 的返回值不一样 pid不同 ppid也不同 未决信号与文🔓 不继承资源利用量清0
        - init进程 是所以进程的祖先进程 pid == 1
        - 调度器的调度策略来决定哪个进程先运行
~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    printf("%d start !\n",getpid());
    fflush(NULL);//记得刷新 否则begin放到缓冲区 父子进程的缓冲区里各有一句begin

    pid_t pid = fork();

    if (pid == 0){
        printf("child %d\n",getpid());
    }else{
        printf("parent %d\n",getpid());
    }
    getchar();
    printf("pid %d end\n",getpid());
    return 0;
}

~~~

~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define LEFT 2
#define RIGHT 200

int main()
{
    pid_t pid = 0;
    int i,j,mark;

    for (i = LEFT;i <= RIGHT;i++){
        pid = fork();
        if (pid == 0){
            mark = 1;
            for (j = 2;j < i/2;j++){
                if (i%j == 0){
                    mark = 0;
                    break;
                }
            }
            if (mark) {
                printf("%d is a primer\n",i);
            }
            exit(0);
        }
    }
    getchar();

    exit(0);
}

~~~

- vfork()

### 进程的消亡以及释放资源
- wait()
- waitpid()

### exec函数族

### 用户权限以及组权限
- 
### 解释器文件

### system()


### 进程会计


### 进程时间

### 守护进程


### 系统日志

@[TOC](文章目录)
