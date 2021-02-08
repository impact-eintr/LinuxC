#include "mypipe.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct mypipe_st {
    int head;
    int tail;
    char data[PIPESIZE];
    int datasize;
    int count_rd;
    int count_wr;
    pthread_mutex_t mut;
    pthread_cond_t cond;
};

pipe_t* pipeinit(void)
{
    struct mypipe_st* me;

    me = malloc(sizeof(*me));
    if (me == NULL)
        return NULL;

    me->head = 0;
    me->tail = 0;
    me->datasize = 0;
    me->count_rd = 0;
    me->count_wr = 0;
    pthread_mutex_init(&me->mut, NULL);
    pthread_cond_init(&me->cond, NULL);

    return me;
}
//注册
int pipe_register(pipe_t* ptr, int opmap)
{
    struct mypipe_st* me = ptr;

    pthread_mutex_lock(&me->mut);
    if (opmap & MYPIPE_READ)
        me->count_rd++;
    if (opmap & MYPIPE_WRITE)
        me->count_wr++;

    while (me->count_rd <= 0 || me->count_wr <= 0)
        pthread_cond_wait(&me->cond, &me->mut);

    pthread_mutex_unlock(&me->mut);
    return 0;
}

//注销
int pipe_unregister(pipe_t* ptr, int opmap)
{
    struct mypipe_st* me = ptr;

    pthread_mutex_lock(&me->mut);
    if (opmap & MYPIPE_READ)
        me->count_rd--;
    if (opmap & MYPIPE_WRITE)
        me->count_wr--;

    pthread_mutex_unlock(&me->mut);
    return 0;
}

static int mypipe_readbyte_unlock(struct mypipe_st* me, char* datap)
{
    if (me->datasize <= 0)
        return -1;
    *datap = me->data[me->head];
    me->head = (me->head) % PIPESIZE + 1;
    me->datasize--;
    return 0;
}

int pipe_read(pipe_t* ptr, void* buf, size_t count)
{
    struct mypipe_st* me = ptr;
    int i;

    pthread_mutex_lock(&me->mut);

    while (me->datasize <= 0)
        pthread_cond_wait(&me->cond, &me->mut);

    for (i = 0; i < count; i++) {
        if (mypipe_readbyte_unlock(me, buf + i) != 0)
            break;
    }

    if (me->datasize <= 0 && me->count_wr > 0) {
        pthread_mutex_unlock(&me->mut);
    }

    pthread_cond_broadcast(&me->cond);
    pthread_mutex_unlock(&me->mut);
    return i;
}

int pipe_write(pipe_t*, const void*, size_t);

int pipe_destory(pipe_t* ptr)
{
    struct mypipe_st* me = ptr;
    pthread_mutex_destroy(&me->mut);
    pthread_cond_destroy(&me->cond);
    free(me);
    return 0;
}
