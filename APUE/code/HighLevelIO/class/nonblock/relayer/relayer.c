#include "relayer.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

enum { STAT_R = 1,
    STAT_W,
    STAT_Ex,
    STAT_T };
//定义状态机的数据结构
struct rel_fsm_st {
    int state;
    int sfd;
    int dfd;
    char buf[BUFSIZE];
    int len;
    int pos; //读写指针
    char* errstr;
    int64_t count;
};
//定义任务的结构
struct rel_job_st {
    int job_state;
    int fd1;
    int fd2;
    int fd1_save, fd2_save;
    struct rel_fsm_st fsm12, fsm21;
};

static struct rel_job_st* rel_job[REL_JOBMAX];
static pthread_mutex_t mut_rel_job;//将job数组设置为临界区
static pthread_once_t init_once = PTHREAD_ONCE_INIT;

static void fsm_driver(struct rel_fsm_st* fsm)
{
    int ret = 0;
    switch (fsm->state) {

        case STAT_R:
            fsm->len = read(fsm->sfd, fsm->buf, BUFSIZE);
            if (fsm->len == 0) {
                fsm->state = STAT_T;
            } else if (fsm->len < 0) {
                if (errno == EAGAIN)
                  fsm->state = STAT_W;
                else {
                    fsm->errstr = "read()";
                    fsm->state = STAT_Ex;
                }
            } else {
                fsm->pos = 0;
                fsm->state = STAT_W;
            }
            break;

        case STAT_W:
            ret = write(fsm->dfd, fsm->buf + fsm->pos, fsm->len);
            if (ret < 0) {
                if (ret == EAGAIN)
                  fsm->state = STAT_W;
                else
                  fsm->state = STAT_Ex;
            } else {
                fsm->pos += ret; //文件读写位置移动
                fsm->len -= ret;
                if (fsm->len == 0) {
                    fsm->state = STAT_R;
                } else {
                    fsm->errstr = "write()";
                    fsm->state = STAT_W;
                }
            }
            break;

        case STAT_Ex:
            perror(fsm->errstr);
            fsm->state = STAT_Ex;
            break;

        case STAT_T:
            break;

        default:
            break;
    }
}

static void* thr_relay(void* p)
{
    while (1) {
        pthread_mutex_lock(&mut_rel_job);

        for (int i = 0; i < REL_JOBMAX; i++) {
            if (rel_job[i] != NULL) {
                if (rel_job[i]->job_state == STATE_R) {

                    fsm_driver(&rel_job[i]->fsm12);
                    fsm_driver(&rel_job[i]->fsm21);

                    if (rel_job[i]->fsm12.state == STAT_T && rel_job[i]->fsm21.state == STAT_T) {
                        rel_job[i]->job_state = STATE_O;
                    }
                }
            }
        }
        pthread_mutex_unlock(&mut_rel_job);
    }
    return NULL;
}

//单次处理函数,用来推状态机
static void module_load(void)
{
    pthread_t tid_relayer;
    int err;

    err = pthread_create(&tid_relayer, NULL, thr_relay, NULL);
    if (err) {
        fprintf(stderr, "pthread_create):%s\n", strerror(err));
        exit(1);
    }
}

int get_free_pos_unlocked()
{
    for (int i = 0; i < REL_JOBMAX; i++) {
        if (rel_job[i] == NULL)
          return i;
    }
    return -1;
}

int rel_addjob(int fd1, int fd2)
{
    struct rel_job_st* me;

    pthread_once(&init_once, module_load);

    me = malloc(sizeof(*me));
    if (me == NULL)
      return -ENOMEM;

    me->fd1 = fd1;
    me->fd2 = fd2;
    me->job_state = STATE_R;

    //保护现场，保证是非阻塞打开
    me->fd1_save = fcntl(fd1, F_GETFL);   //将文件符保存
    fcntl(me->fd1, me->fd1 | O_NONBLOCK); //保证文件描述符是非阻塞的
    me->fd2_save = fcntl(fd2, F_GETFL);
    fcntl(me->fd2, me->fd2 | O_NONBLOCK);

    //设置状态机
    me->fsm12.state = STATE_R;
    me->fsm12.sfd = fd1;
    me->fsm12.dfd = fd2;

    me->fsm21.state = STATE_R;
    me->fsm21.sfd = fd2;
    me->fsm21.dfd = fd1;

    pthread_mutex_lock(&mut_rel_job);
    int pos = get_free_pos_unlocked();
    if (pos < 0) {
        pthread_mutex_unlock(&mut_rel_job);
        fcntl(me->fd1, F_SETFL, me->fd1_save);
        fcntl(me->fd2, F_SETFL, me->fd2_save);
        free(me);
    }
    pthread_mutex_unlock(&mut_rel_job);

    return 0;
}

#if 0
int rel_canceljob(int jo0b);

int rel_waitjob(int id,struct rel_stat_st *);;

int rel_statjob(int id,struct rel_stat_st *);
#endif
