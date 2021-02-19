# 高级IO
## 非阻塞IO
不死等，检测到没有回应，一会儿再检测



### 有限状态机编程
**可以解决复杂流程问题**
- 简单流程 自然流程是结构化的
- 复杂流程 自然流程不是结构化的

### 数据中继

~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

#define BUFSIZE 1024
#define TTY1 "/dev/tty11"
#define TTY2 "/dev/tty12"

enum {
    STATE_R = 1,
    STATE_W,
    STATE_Ex,
    STATE_T
};

struct fsm_st{
    int state;
    int sfd;//源
    int dfd;//目标
    char buf[BUFSIZE];
    int len;
    int pos;
    //出错原因
    char *errMess;
};

static void fsm_driver(struct fsm_st* fsm){
    int ret;

    switch(fsm->state){
        case STATE_R:
            fsm->len = read(fsm->sfd,fsm->buf,BUFSIZE);
            if (fsm->len == 0){
                fsm->state = STATE_T;//正常结束
            }else if (fsm->len < 0){
                if (errno == EAGAIN)
                  fsm->state = STATE_R;//假错 保留状态
                else{
                    fsm->state = STATE_Ex;//真正出错
                    fsm->errMess = "读取失败";
                }

            }else{
                fsm->pos = 0;
                fsm->state = STATE_W;//成功读入 转换状态
            }

            break;
        case STATE_W:
            ret = write(fsm->dfd,fsm->buf+fsm->pos,fsm->len);
            if (ret < 0){
                if (errno == EAGAIN){
                    fsm->state = STATE_W;
                }else{
                    fsm->errMess = "写入失败";
                    fsm->state = STATE_Ex;
                }
            }else{
                //坚持写够
                fsm->pos += ret;
                fsm->len -= ret;
                if (fsm->len == 0){
                    fsm->state = STATE_R;
                }else{
                    fsm->state = STATE_W;
                }
            }

            break;
        case STATE_Ex:
            perror(fsm->errMess);
            fsm->state = STATE_T;
            break;
        case STATE_T:
            /*do sth*/
            break;
        default:
            abort();
            break;
    }

}

static void relay(int fd1,int fd2){
    int old_fd1,old_fd2;

    struct fsm_st fsm12,fsm21;//读左写右 读右写左

    old_fd1 = fcntl(fd1,F_GETFL);
    fcntl(fd1,F_SETFL,old_fd1|O_NONBLOCK);

    old_fd2 = fcntl(fd2,F_GETFL);
    fcntl(fd2,F_SETFL,old_fd2|O_NONBLOCK);

    fsm12.state = STATE_R;
    fsm12.sfd = fd1;
    fsm12.dfd = fd2;

    fsm21.state = STATE_R;
    fsm21.sfd = fd2;
    fsm21.dfd = fd1;

    while(fsm12.state != STATE_T || fsm21.state != STATE_T){//状态机没有停转
        fsm_driver(&fsm12);
        fsm_driver(&fsm21);

    }
    

    //恢复原来的文件描述符状态
    fcntl(fd1,F_SETFL,old_fd1);
    fcntl(fd2,F_SETFL,old_fd2);

}

int main()
{
    int fd1,fd2;
    //假设用户使用阻塞的方式打开设备
    fd1 = open(TTY1,O_RDWR);
    if (fd1 < 0){
        perror("open()");
        exit(1);
    }
    write(fd1,"TTY1\n",5);

    fd2 = open(TTY2,O_RDWR|O_NONBLOCK);
    if (fd2 < 0){
        perror("open()");
        close(fd1);
        exit(1);
    }
    write(fd2,"TTY2\n",5);

    relay(fd1,fd2);

    close(fd1);
    close(fd2);
    exit(0);
}

~~~

## IO多路转接
**解决`IO密集型任务`中盲等的问题，监视文件描述符的行为，当当前文件描述符发生了我们感兴趣的行为时，才去做后续操作**

- select() 兼容性好 但设计有缺陷 **以事件为单位组织文件描述符**

可以实现安全的休眠(替代sleep)
~~~ c


~~~
- poll() **以文件描述符为单位组织事件**
- epoll() linux的poll封装方案 不可移植
        
## 其他读写函数

## 存储映射IO

## 文件锁
