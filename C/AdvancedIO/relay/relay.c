#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>

#include "relay.h"

#define BUFSIZE 1024

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
    int64_t count;
    int len;
    int pos;
    //出错原因
    char *errMess;
};

struct job_st{
    int job_state;
    int fd1,old_fd1;
    int fd2,old_fd2;
    struct fsm_st fsm12,fsm21;

};

//全局任务数组
static struct job_st* jobs[JOBMAX];
//保护任务数组的互斥量
static pthread_mutex_t job_mutex = PTHREAD_MUTEX_INITIALIZER;
//单次初始化变量
static pthread_once_t job_once = PTHREAD_ONCE_INIT;


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

static int get_free_pos_unlocked(){
    for (int i = 0;i < JOBMAX;i++){
        if (jobs[i] == NULL)
          return  i;
    }
    return -1;
}

static void *handler(void *p){
    while(1){
        pthread_mutex_lock(&job_mutex);
        for (int i = 0;i < JOBMAX;i++){
            if (jobs[i] != NULL){
                if (jobs[i]->job_state == STATE_RUNNING){
                    //尝试发动状态机
                    fsm_driver(&jobs[i]->fsm12);
                    fsm_driver(&jobs[i]->fsm21);
                    if(jobs[i]->fsm12.state == STATE_T&&
                                jobs[i]->fsm21.state == STATE_T){
                        jobs[i]->job_state = STATE_OVER;//两个状态机都已经停止 任务结束
                    }
                }
            }
            pthread_mutex_unlock(&job_mutex);
        }
    }
    pthread_exit(NULL);
}

static void  module_load(){
    pthread_t ptid;
    int err= pthread_create(&ptid,NULL,handler,NULL);
    if (err){
        fprintf(stderr,"pthread_create() %s\n",strerror(err));
        exit(1);
    }
}

//return >= 0 get job id
//       == -EINVAL 参数非法
//       == -ENOSPC 任务数组满
//       == -ENOMEM 内存分配出错
int relay_addjob(int fd1,int fd2){

    pthread_once(&job_once,module_load);

    int pos;
    struct job_st *job;
    job = malloc(sizeof(*job));
    if (job == NULL){
        return -ENOMEM;
    }

    job->fd1 = fd1;
    job->fd2 = fd2;
    job->job_state = STATE_RUNNING;

    job->old_fd1 = fcntl(job->fd1,F_GETFL);
    fcntl(fd1,F_SETFL,job->old_fd1|O_NONBLOCK);

    job->old_fd2 = fcntl(job->fd2,F_GETFL);
    fcntl(fd2,F_SETFL,job->old_fd2|O_NONBLOCK);

    job->fsm12.state = STATE_R;
    job->fsm12.sfd = fd1;
    job->fsm12.dfd = fd2;

    job->fsm21.state = STATE_R;
    job->fsm21.sfd = fd2;
    job->fsm21.dfd = fd1;

    pthread_mutex_lock(&job_mutex);
    pos = get_free_pos_unlocked();
    if (pos < 0){
        pthread_mutex_unlock(&job_mutex);
        //恢复原来的文件描述符状态
        fcntl(job->fd1,F_SETFL,job->old_fd1);
        fcntl(job->fd2,F_SETFL,job->old_fd2);
        free(job);
        return -ENOSPC;
    }
    jobs[pos] = job;

    pthread_mutex_unlock(&job_mutex);

    return pos;
}

//return == 0 取消成功
//       == -EINVAL 单数非法
//       == -EBUSY 任务取消过了
int relay_canceljob(int id);

//return == 0 返回状态
//       == -EINVAL 参数非法
int relay_waitjob(int id);

//reutnr == 0 指定任务状态已返回
int relay_statjob(int id,struct relay_stat_st *state);


