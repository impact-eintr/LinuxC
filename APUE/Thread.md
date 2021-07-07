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

- pthread_setcancelstate() 设置是否允许取消
- pthread_testcancel() 什么都不做 本身就是一个取消点

- 进程异常终止的条件之一
    - **最后一个线程对其取消请求作出响应**

~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

#define N 5
#define LEFT 30000000
#define RIGHT 30000200


static void *handler(void *p){
    int n = *(int *)p;
    int i,j,mark;
    for (i = LEFT+n;i <= RIGHT;i+=N){
        mark = 1;
        for (j = 2;j <= i/2;j++){
            if (i%j == 0){
                mark = 0;
                break;
            }
        }
        if (mark) {
            printf("%d is a primer [%d]\n",i,n);
        }
    }
    pthread_exit(p);

}

//交叉算法计算 池类算法涉及到竞争
int main()
{
    pthread_t Ptid[N];
    void *ptr = NULL;

    for (int n = 0;n < N;n++){
        int *num = malloc(sizeof(int));
        *num = n;
        int err = pthread_create(Ptid+n,NULL,handler,num);
        if (err){
            fprintf(stderr,"%s\n",strerror(err));
            exit(1);
        }
    }

    int n;
    for (n =0 ;n < N;n++){
        pthread_join(Ptid[n],ptr);
        free(ptr);
    }

    exit(0);
}

~~~

## 线程同步
#### 互斥量

**锁住的是一段代码而不是一个变量**
- pthread_mutex_t
- pthread_mutex_init()
- pthread_mutex_destory()
- pthread_mutex_lock()
- pthread_mutex_trylock()
- pthread_mutex_unlock()
- pthread_once() **动态模块的单词初始化函数**

互斥量像是`bool`，非黒即白，没有共享性

~~~ c
//互斥量
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void *handler(void *p){
    FILE *fp = fopen(FNAME,"r+");
    char buf[BUFSIZE];

    if(fp == NULL){
        perror("fopen()");
        exit(1);
    }

    //进入临界区
    pthread_mutex_lock(&mutex);
    fgets(buf,BUFSIZE,fp);
    fseek(fp,0,SEEK_SET);
    sleep(1);
    fprintf(fp,"%d\n",atoi(buf)+1);
    fclose(fp);
    pthread_mutex_unlock(&mutex);
    //离开临界区

    pthread_exit(NULL);
}

int main()
{
    pthread_t Ptid[THRNUM];

    for (int i = 0;i < THRNUM;i++){
        int err = pthread_create(Ptid+i,NULL,handler,NULL);
        if (err){
           fprintf(stderr,"%s\n",strerror(err));
           exit(1);
        }
    }

    for (int i = 0;i < THRNUM;i++){
        pthread_join(Ptid[i],NULL);
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}

~~~

~~~ c
//循环打印 abcd
#define N 4
#define LEFT 30000000
#define RIGHT 30000200

static pthread_mutex_t mutex_arr[N];


static void *handler(void *p){
    int n = *(int *)p;
    int ch = '1'+n;

    while(1){
        pthread_mutex_lock(mutex_arr+n);
        write(1,&ch,1);
        pthread_mutex_unlock(mutex_arr+(n+1)%N);
    }
    pthread_exit(p);
}

int main()
{
    pthread_t Ptid[N];
    void *ptr = NULL;

    for (int n = 0;n < N;n++){
        int *num = malloc(sizeof(int));
        *num = n;

        pthread_mutex_init(mutex_arr+n,NULL);
        pthread_mutex_lock(mutex_arr+n);

        int err = pthread_create(Ptid+n,NULL,handler,num);
        if (err){
            fprintf(stderr,"%s\n",strerror(err));
            exit(1);
        }
    }

    pthread_mutex_unlock(mutex_arr+0);
    alarm(3) ;   

    int n;
    for (n =0 ;n < N;n++){
        pthread_join(Ptid[n],ptr);
        free(ptr);
    }

    exit(0);
}

~~~

~~~ c
//池类算法算质数
#define THRNUM 3
#define LEFT 30000000
#define RIGHT 30000200

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;;
static int num = 0;

static void *handler(void *p){
    int task,mark;

    while(1){
        pthread_mutex_lock(&mutex);
        while(num == 0){
            pthread_mutex_unlock(&mutex);
            sched_yield();
            pthread_mutex_lock(&mutex);
        }
    
        if (num == -1){
            pthread_mutex_unlock(&mutex);
            break;
        }

        task = num;
        num = 0;//成功领取任务
        pthread_mutex_unlock(&mutex);

        mark = 1;
        for (int j = 2;j <= task/2;j++){
            if (task%j == 0){
                mark = 0;
                break;
            }
        }
        if (mark) {
            printf("[%d] %d is a priamer\n",(int)p,task);
        }
    }

    pthread_exit(NULL);
}

//池类算法
int main()
{
    pthread_t Ptid[THRNUM];

    for (int n = 0;n < THRNUM;n++){
        int err = pthread_create(Ptid+n,NULL,handler,(void *)n);
        if (err){
            fprintf(stderr,"%s\n",strerror(err));
            exit(1);
        }
    }

    for (int i = LEFT;i <= RIGHT;i++){
        pthread_mutex_lock(&mutex);
        
        //任务没有被领取
        while(num != 0){
            pthread_mutex_unlock(&mutex);
            sched_yield();
            pthread_mutex_lock(&mutex);
        }
        //任务已经成功下发
        num = i;
        pthread_mutex_unlock(&mutex);
    }
    
    pthread_mutex_lock(&mutex);
    //任务没有被领取
    while(num != 0){
        pthread_mutex_unlock(&mutex);
        sched_yield();
        pthread_mutex_lock(&mutex);
    }
    //任务已经成功下发
    num = -1;
    pthread_mutex_unlock(&mutex);

    int n;
    for (n =0 ;n < THRNUM;n++){
        pthread_join(Ptid[n],NULL);
    }

    pthread_mutex_destroy(&mutex);

    exit(0);
}

~~~
**头文件 与 测试主函数 请见信号篇的 令牌桶**
~~~ c
//令牌桶
struct mytbf_st{
    int csp;
    int burst;
    int token;
    int pos;//任务列表的下标
    pthread_mutex_t mut;
};

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_t ptid;
static pthread_once_t pth_once = PTHREAD_ONCE_INIT;

static struct mytbf_st *job[MYTBF_MAX];
static volatile int inited = 0;

static int get_free_pos_unlocked(){
    for (int i = 0;i < MYTBF_MAX;i++){
        if (job[i] == NULL)
          return  i;
    }
    return -1;
}

//线程处理函数
static void *handler(void *p){
    struct timespec ts;
    ts.tv_sec = 1;
    ts.tv_nsec = 0;

    while(1){
        pthread_mutex_lock(&mutex);
        for (int i = 0;i < MYTBF_MAX;i++){
            if (job[i] != NULL){
                pthread_mutex_lock(&job[i]->mut);
                job[i]->token += job[i]->csp;
                if (job[i]->token > job[i]->burst){
                    job[i]->token = job[i]->burst;
                }
                pthread_mutex_unlock(&job[i]->mut);
            }
        }
        pthread_mutex_unlock(&mutex);
        nanosleep(&ts,NULL);

    }
    pthread_exit(NULL);
}

//卸载线程处理模块
static void mod_unload(){
    pthread_cancel(ptid);
    pthread_join(ptid,NULL);
    for (int i = 0;i < MYTBF_MAX;i++){
        if (job[i] != NULL){
            mytbf_destroy(job[i]);
        }
        free(job[i]);
    }

    pthread_mutex_destroy(&mutex);
}

//装载线程处理模块
static void mod_load(){

    int err = pthread_create(&ptid,NULL,handler,NULL);
    if (err){
        fprintf(stderr,"%s\n",strerror(err));
    }

    atexit(mod_unload);
}

mytbf_t *mytbf_init(int cps,int burst){
    struct mytbf_st *tbf;

    pthread_once(&pth_once,mod_load);

    tbf = malloc(sizeof(*tbf));
    if (tbf == NULL){
        return NULL;
    }
    tbf->token = 0;
    tbf->csp = cps;
    tbf->burst = burst;
    pthread_mutex_init(&tbf->mut,NULL);

    pthread_mutex_lock(&mutex);
    //将新的tbf装载到任务组中
    int pos = get_free_pos_unlocked();
    if (pos == -1){
        free(tbf);
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    tbf->pos = pos;
    job[pos] = tbf;
    
    pthread_mutex_unlock(&mutex);

    return tbf;
}

//获取token ptr是一个 void * size是用户想要获取的token数
int mytbf_fetchtoken(mytbf_t *ptr,int size){
    struct mytbf_st *tbf = ptr;

    if (size <= 0){
        return -EINVAL;
    }
    
    //有token继续
    pthread_mutex_lock(&tbf->mut);
    while (tbf->token <= 0){
        pthread_mutex_unlock(&tbf->mut);
        sched_yield();
        pthread_mutex_lock(&tbf->mut);
    }

    int n =tbf->token<size?tbf->token:size;
    tbf->token -= n;

    pthread_mutex_unlock(&tbf->mut);
    //用户获取了 n 个token
    return n;
}

//归还token ptr是一个 void *
int mytbf_returntoken(mytbf_t *ptr,int size){
    struct mytbf_st *tbf = ptr;

    if (size <= 0){
        return -EINVAL;
    }
    pthread_mutex_lock(&tbf->mut);
    tbf->token += size;
    if (tbf->token > tbf->burst)
        tbf->token = tbf->burst;
    pthread_mutex_unlock(&tbf->mut);

    return size;
}

int mytbf_destroy(mytbf_t *ptr){
    struct mytbf_st *tbf = ptr;
    pthread_mutex_lock(&mutex);
    job[tbf->pos] = NULL;
    pthread_mutex_unlock(&mutex);

    pthread_mutex_destroy(&tbf->mut);
    free(tbf);
    return 0;
}
~~~

#### 条件变量

- pthread_cond_t
- pthread_cond_init()
- pthread_cond_destory()
- pthread_cond_wait() **等通知 + 抢锁**
- pthread_cond_broadcast() 广播给所有线程
- pthread_cond_signal() 通知任意一个线程

条件变量可以解决 互斥量进行盲等的问题 即实现了通知法，**通知互斥量什么时候上锁**

~~~ c
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;;
static int num = 0;

static void *handler(void *p){
    int task,mark;

    while(1){
        pthread_mutex_lock(&mutex);
        while(num == 0){
            pthread_cond_wait(&cond,&mutex);
        }
    
        if (num == -1){
            pthread_mutex_unlock(&mutex);
            break;
        }

        task = num;
        num = 0;//成功领取任务
        //通知所有线程任务被领取走了
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);

        mark = 1;
        for (int j = 2;j <= task/2;j++){
            if (task%j == 0){
                mark = 0;
                break;
            }
        }
        if (mark) {
            printf("[%d] %d is a priamer\n",*(int *)p,task);
        }
    }

    pthread_exit(NULL);
}

//池类算法
int main()
{
    pthread_t Ptid[THRNUM];

    for (int n = 0;n < THRNUM;n++){
        int *num = malloc(sizeof(int));
        *num = n;
        int err = pthread_create(Ptid+n,NULL,handler,num);
        if (err){
            fprintf(stderr,"%s\n",strerror(err));
            exit(1);
        }
    }

    for (int i = LEFT;i <= RIGHT;i++){
        pthread_mutex_lock(&mutex);
        
        //任务没有被领取
        while(num != 0){
            pthread_cond_wait(&cond,&mutex);
        }
        //任务已经成功下发
        num = i;
        //叫醒任意一个线程执行任务
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    
    pthread_mutex_lock(&mutex);
    //任务没有被领取
    while(num != 0){
        pthread_cond_wait(&cond,&mutex);
    }
    //任务已经成功下发
    num = -1;
    //广播给所有下游线程 任务结束
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);

    int n;
    for (n =0 ;n < THRNUM;n++){
        pthread_join(Ptid[n],NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    exit(0);
}

~~~

#### 信号量

**通过互斥量与条件变量的配合我们可以实现信号量 信号量像是一个激活函数 当这个变量超过阈值时 将会触发条件变量给互斥量上锁**

~~~ c
#include "mysem.h"

struct mysem_st{
    int vaclue;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

mysem_t *mysem_init(int initval){
    struct mysem_st *sem;
    sem = malloc(sizeof(*sem));

    if (sem == NULL){
        return NULL;
    }
    
    sem->vaclue = initval;
    pthread_mutex_init(&sem->mutex,NULL);
    pthread_cond_init(&sem->cond,NULL);

    return sem;
}

int mysem_add(mysem_t *ptr,int n){

    struct mysem_st *sem = ptr;

    pthread_mutex_lock(&sem->mutex);
    sem->vaclue += n;
    pthread_cond_broadcast(&sem->cond);
    pthread_mutex_unlock(&sem->mutex);

    return n;
}

int mysem_sub(mysem_t *ptr,int n){
    struct mysem_st *sem = ptr;

    pthread_mutex_lock(&sem->mutex);

    while(sem->vaclue < n){
        pthread_cond_wait(&sem->cond,&sem->mutex);
    }
    sem->vaclue -= n;
    pthread_mutex_unlock(&sem->mutex);
    
    return n;
}

int mysem_destory(mysem_t *ptr){
    struct mysem_st *sem = ptr;

    pthread_mutex_destroy(&sem->mutex);
    pthread_cond_destroy(&sem->cond);
    free(sem);

    return 0;
}

~~~

~~~ c

#define THRNUM 20
#define N 5
#define LEFT 30000000
#define RIGHT 30000200

static mysem_t *sem;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;;
static int num = 0;

static void *handler(void *p){
    int task,mark= 0;

    pthread_mutex_lock(&mutex);
    while(num == 0){
        pthread_cond_wait(&cond,&mutex);
    }
    
    if (num == -1){
        pthread_mutex_unlock(&mutex);
        mysem_add(sem,1);
        pthread_exit(NULL);
    }

    task = num;
    num = 0;//成功领取任务
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);

    mark = 1;
    for (int j = 2;j <= task/2;j++){
        if (task%j == 0){
            mark = 0;
            //归还计算资源
            sleep(1);
            mysem_add(sem,1);
            pthread_exit(NULL);
        }
    }
    if (mark) {
        printf("[%d] %d is a priamer\n",*(int *)p,task);
    }

    sleep(1);

    //归还计算资源
    mysem_add(sem,1);

    pthread_exit(NULL);
}

//池类算法
int main()
{
    pthread_t Ptid[THRNUM];
    sem = mysem_init(N);//初始化计算资源

    for (int i = LEFT;i <= RIGHT;i++){
        mysem_sub(sem,1);//消耗一个计算资源
        int *ptid = malloc(sizeof(int));
        *ptid = i-LEFT;
        int err = pthread_create(Ptid+(i-LEFT)%THRNUM,NULL,handler,ptid);
        if (err){
            fprintf(stderr,"%s\n",strerror(err));
            exit(1);
        }

        pthread_mutex_lock(&mutex);
        
        //任务没有被领取
        while(num != 0){
            pthread_cond_wait(&cond,&mutex);
        }
        //任务已经成功下发
        num = i;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    
    pthread_mutex_lock(&mutex);
    //任务没有被领取
    while(num != 0){
        pthread_cond_wait(&cond,&mutex);
    }
    //任务已经成功下发
    num = -1;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);

    int n;
    for (n =0 ;n < THRNUM;n++){
        pthread_join(Ptid[n],NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    exit(0);
}

~~~

#### 读写锁

~~~ c

~~~

## 线程属性
- pthread_attr_init()
- pthread_attr_destory()
- pthread_attr_setstacksize()
其他请见 `man pthread_attr_init` 的 `see also`

### 线程同步的属性
#### 互斥量属性
- pthread_mutexattr_init()
- pthread_mutexattr_destory()
- **clone** 进程 线程 不分家a
    - 跨进程设置锁
    - pthread_mutexattr_getshared()
    - pthread_mutexattr_setshared()
- pthread_mutexattr_gettype()
- pthread_mutexattr_settype()

#### 条件变量属性
- pthread_condattr_init()
- pthread_condattr_destory()

### 重入(reentry)
#### 多线程中的IO
- getchar_unlocked

### 线程与信号
- pthread_sigmask()
- sigwait()
- pthread_kill()

## openmp

~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


int main()
{
#pragma omp parallel sections
{
#pragma omp section
    printf("Hello %d\n",omp_get_thread_num());
#pragma omp section
    printf("World %d\n",omp_get_thread_num());
}
    exit(0);
}

~~~
