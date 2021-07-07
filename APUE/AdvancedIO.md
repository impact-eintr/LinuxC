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
可以实现安全的休眠(替代sleep)

- select() 兼容性好 但设计有缺陷 **以事件为单位组织文件描述符**
- `nfds`的类型问题
- 参数没有`const`修饰 也就是函数会修改 fdset 任务和结果放在一起
- 监视的事件泰国单一 读 写 异常(异常的种类非常多)

~~~ c

static void relay(int fd1,int fd2){
    int old_fd1,old_fd2;

    struct fsm_st fsm12,fsm21;//读左写右 读右写左

    fd_set rset,wset;

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
        //布置监视任务
        FD_ZERO(&rset);
        FD_ZERO(&wset);
        if (fsm12.state == STATE_R)
          FD_SET(fsm12.sfd,&rset);
        if (fsm12.state == STATE_W)
          FD_SET(fsm12.sfd,&wset);
        if (fsm21.state == STATE_R)
          FD_SET(fsm21.sfd,&rset);
        if (fsm21.state == STATE_W)
          FD_SET(fsm21.sfd,&wset);

        //监视
        struct  timeval ts;
        ts.tv_sec = 0;
        ts.tv_usec= 2;
        int maxfd = fd1>fd2?fd1:fd2;
        if (fsm12.state < STATE_AUTO ||fsm21.state < STATE_AUTO){
            if (select(maxfd+1,&rset,&wset,NULL,&ts) < 0){
                if (errno == EINTR)
                  continue;
                perror("select()");
                exit(1);
            }
        }

        //查看监视结果
        if (FD_ISSET(fd1,&rset) || FD_ISSET(fd2,&wset) || fsm12.state > STATE_AUTO){
            fsm_driver(&fsm12);
        }
        if (FD_ISSET(fd2,&rset) || FD_ISSET(fd1,&wset) || fsm21.state > STATE_AUTO){
            fsm_driver(&fsm21);
        }
    }

    //恢复原来的文件描述符状态
    fcntl(fd1,F_SETFL,old_fd1);
    fcntl(fd2,F_SETFL,old_fd2);

}
~~~

- poll()   
	- **wait for some event on a file descriptor （以文件描述符为单位组织事件）**
    - int poll(struct pollfd *fds,nfds,int timeout);

~~~ c
#include <poll.h>
static void relay(int fd1,int fd2){
    int old_fd1,old_fd2;

    struct fsm_st fsm12,fsm21;//读左写右 读右写左
    
    struct pollfd pfd[2];

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

    pfd[0].fd = fd1;
    pfd[1].fd = fd2;

    while(fsm12.state != STATE_T || fsm21.state != STATE_T){//状态机没有停转
        //布置监视任务
        pfd[0].events = 0;//事件清零
        if (fsm12.state == STATE_R)//1可读
          pfd[0].events |= POLLIN;
        if (fsm21.state == STATE_W)//1可写
          pfd[0].events |= POLLOUT;

        pfd[1].events = 0;//事件清零
        if (fsm21.state == STATE_R)//2可读
          pfd[1].events |= POLLIN;
        if (fsm12.state == STATE_W)//2可写
          pfd[1].events |= POLLOUT;

        //监视
        if (fsm12.state < STATE_AUTO ||fsm21.state < STATE_AUTO){
            while (poll(pfd,2,-1) < 0){
                if (errno == EINTR)
                  continue;
                perror("poll()");
                exit(1);
            }
        }

        //查看监视结果
        if (pfd[0].revents&POLLIN || pfd[1].revents&POLLOUT || fsm12.state > STATE_AUTO){
            fsm_driver(&fsm12);
        }
        if (pfd[1].revents&POLLIN || pfd[0].revents&POLLOUT || fsm21.state > STATE_AUTO){
            fsm_driver(&fsm21);
        }
    }

    //恢复原来的文件描述符状态
    fcntl(fd1,F_SETFL,old_fd1);
    fcntl(fd2,F_SETFL,old_fd2);

}
~~~

- epoll     linux的poll封装方案 不可移植
    - epoll_create()
    - epoll_ctl(int epfd,int op,int fd,struct epoll_event *event) 指定epool实例 指定操作动作  指定操作的文件描述符 指定事件
	~~~ c
	typedef union epoll_data {
	    void        *ptr;
	    int          fd;
	    uint32_t     u32;
	    uint64_t     u64;
	} epoll_data_t;
	
	struct epoll_event {
	    uint32_t     events;      /* Epoll events */
	    epoll_data_t data;        /* User data variable */
	};
	
	~~~
	- epoll不仅可以使用文件描述符 也可以记录相关的信息
    - epoll_wait()

~~~ c
#include <sys/epoll.h>
static void relay(int fd1,int fd2){
    int old_fd1,old_fd2;

    struct fsm_st fsm12,fsm21;//读左写右 读右写左
    
    struct epoll_event ev;

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

    int epfd = epoll_create(2);
    if (epfd < 0){
        perror("epoll_create()");
        exit(1);
    }
    ev.data.fd = fd1;
    ev.events = 0;
    epoll_ctl(epfd,EPOLL_CTL_ADD,fd1,&ev);
    ev.data.fd = fd2;
    ev.events = 0;
    epoll_ctl(epfd,EPOLL_CTL_ADD,fd2,&ev);

    while(fsm12.state != STATE_T || fsm21.state != STATE_T){//状态机没有停转
        ev.data.fd = fd1;
        ev.events = 0;
        //布置监视任务
        if (fsm12.state == STATE_R)//1可读
          ev.events |= EPOLLIN;
        if (fsm21.state == STATE_W)//1可写
          ev.events |= EPOLLOUT;
        epoll_ctl(epfd,EPOLL_CTL_MOD,fd1,&ev);

        ev.data.fd = fd2;
        ev.events = 0;
        if (fsm21.state == STATE_R)//2可读
          ev.events |= EPOLLIN;
        if (fsm12.state == STATE_W)//2可写
          ev.events |= EPOLLOUT;
        epoll_ctl(epfd,EPOLL_CTL_MOD,fd2,&ev);

        //监视
        if (fsm12.state < STATE_AUTO ||fsm21.state < STATE_AUTO){
            while (epoll_wait(epfd,&ev,1,-1) < 0){
                if (errno == EINTR)
                  continue;
                perror("poll()");
                exit(1);
            }
        }

        //查看监视结果
        if (ev.data.fd == fd1 && ev.events&POLLIN ||
                ev.data.fd == fd2 && ev.events&POLLOUT ||
                    fsm12.state > STATE_AUTO){
            fsm_driver(&fsm12);
        }
        if (ev.data.fd == fd2 && ev.events&POLLIN ||
                ev.data.fd == fd1 && ev.events&POLLOUT ||
                    fsm21.state > STATE_AUTO){
            fsm_driver(&fsm21);
        }
    }

    //恢复原来的文件描述符状态
    fcntl(fd1,F_SETFL,old_fd1);
    fcntl(fd2,F_SETFL,old_fd2);

    close(epfd)

}


~~~
## 其他读写函数
- readv()
- writev()

## 存储映射IO
把内存中的内容 或者 某一个文件的内容 映射到当前进程空间中来
- mmap(void *addr,size_t length,int prot,int flags,int fd,odd_t offset);
    - 匿名映射可以实现malloc的功能
- munmap(void *addr,size_t length)

~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc,char **argv)
{
    int fd;
    struct stat statres;
    char *str = NULL;
    int count = 0;

    if (argc < 2){
        fprintf(stderr,"Usage...\n");
        exit(1);
    }

    
    fd = open(argv[1],O_RDONLY);
    if (fd < 0){
        perror("open()");
        exit(1);
    }

    if (fstat(fd,&statres)< 0){
        perror("fstat()");
        exit(1);
    }

    str = mmap(str,statres.st_size,PROT_READ,MAP_SHARED,fd,0);
    if (str == MAP_FAILED){
        perror("mmap()");
        exit(1);
    }

    close(fd);//映射结束
    
    for (int i = 0;i < statres.st_size;i++){
        if (str[i] == 'a'){
            count++;
        }
    }
    printf("count %d\n",count);

    munmap(str,statres.st_size);

    exit(0);
}

~~~

## 文件锁
- fcntl()
- lockf()
- flock()

~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>

#define THRNUM 20
#define FNAME "/tmp/out"
#define BUFSIZE 1024

static void handler(){
    FILE *fp = fopen(FNAME,"r+");
    char buf[BUFSIZE];

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

    //离开临界区
    lockf(fd,F_ULOCK,0);

    fclose(fp);
}

int main()
{
    pid_t pid;

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

    return 0;
}

~~~
