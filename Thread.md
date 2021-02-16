# 线程

## 线程的概念
会话是用来承载进程组的，里面可以有一个或多个进程，一个线程中可以有一个或多个线程
**线程的本质就是一个正在运行的函数 ，线程没有主次之分(main函数 也只是一个main线程)，多个线程之间共享内存，**

 **`posix`线程是一套标准，而不是实现，我们主要讨论这套标准**
- 线程标识 `pthead_t` 类型不确定
- pthread_equal()
- pthread_self()

## 线程的一生
### 线程的创建
- pthread_create()

~~~ c#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

static void *func(void *p){
    puts("thread is working");
    return NULL;
}

int main()
{
    printf("Begin\n");
    pthread_t ptid;

    int err = pthread_create(&ptid,NULL,func,NULL);
    if (err){
        fprintf(stderr,"pthread_create err %s\n",strerror(err));
        exit(1);
    }
    sleep(1);
    printf("End\n");
    exit(0);
}


~~~
**线程的调度取决于调度器的测略**

### 线程的终止
1. **线程从启动例程返回，返回值就是线程的退出码**
2. **线程可以被同一进程的其他线程取消**
3. **线程调用`pthread_exit()`函数**

- pthread_exit();

~~~ c
static void *func(void *p){
    puts("thread is working");
    pthread_exit(NULL);
}

int main()
{
    printf("Begin\n");
    pthread_t ptid;

    int err = pthread_create(&ptid,NULL,func,NULL);
    if (err){
        fprintf(stderr,"pthread_create err %s\n",strerror(err));
        exit(1);
    }
    sleep(1);
    printf("End\n");
    exit(0);
}


~~~

- pthread_join() 先当于进程的 wait()

~~~ c
static void *func(void *p){
    puts("thread is working");
    pthread_exit(NULL);
}

int main()
{
    printf("Begin\n");
    pthread_t ptid;

    int err = pthread_create(&ptid,NULL,func,NULL);
    if (err){
        fprintf(stderr,"pthread_create err %s\n",strerror(err));
        exit(1);
    }
    
    pthread_join(ptid,NULL);
    printf("End\n");
    exit(0);
}
~~~

### 线程/栈的清理
- pthread_cleanup_push()
- pthread_cleanup_pop()

**类似钩子函数，程序只要正常终止，钩子函数就会被逆序调用，push 与 pop 可以指定操作**

~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

static void cleanup(void *p){
    puts(p);
}

static void *func(void *p){
    puts("Thread is working");

    pthread_cleanup_push(cleanup,"1");//这个函数是宏
    pthread_cleanup_push(cleanup,"2");
    pthread_cleanup_push(cleanup,"3");
    
    //pthread_exit(NULL);
    //下面的内容执行不到但是不会报错 会按照全为 1 处理

    pthread_cleanup_pop(1);//语法结构一定要对等
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(1);

    puts("push over");

    pthread_exit(NULL);
}

int main()
{
    pthread_t ptid;
    printf("Start !\n");

    int err = pthread_create(&ptid,NULL,func,NULL);//ptid 属性 处理函数 传参
    if (err){
        fprintf(stderr,"pthread_create() : %s\n",strerror(err));
        exit(1);
    }

    pthread_join(ptid,NULL);

    printf("End !\n");
    exit(0);
}

~~~

### 线程的取消选项
- 多线程任务 有时需要取消部分任务(线程)
- pthread_cancel()
- 取消有2种状态 
    - 不允许
    - 允许
        - 异步cancel
        - **推迟cancel(默认) 推迟到cancel点再响应**
        - **cancel点 ： POSIX定义的cancel点 都是可能引发阻塞的系统调用**
~~~ c
//示意

static void cleanup(void *){
    close(*p);
}

//cancel点
fd1 = open();
if (fd1 < 0){
    perror();
    exit(1);//只要调用exit 不管是 ezit(1) 还是 exit(0) 都是正常终止 都会执行线程清理函数
}
//在这收到一个取消请求 但因为下面的代码没有阻塞的系统调用 所以不会响应
pthread_cleanup_push(cleanup,&fd);
pthread)cleanup_pop(1);


//cancel点 下面的open是阻塞的系统调用 对上面的取消请求做出响应

fd2 = open();
if (fd2 < 0){
    perror();
    exit(1);
}
~~~

- 进程异常终止的条件之一
    - **最后一个线程对其取消请求作出响应**
## 线程同步

## 线程属性
### 线程同步的属性

## 重入

### 线程与信号


### 线程与fork

