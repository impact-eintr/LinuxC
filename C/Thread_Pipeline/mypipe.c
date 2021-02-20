#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#include "mypipe.h"

struct mypipe_st{
    int head;
    int tail;
    char data[PIPESIZE];
    int datasize;
    int count_reader;
    int count_writer;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

mypipe_t *mypipe_init(){
    struct mypipe_st *pipe;
    pipe = malloc(sizeof(*pipe));
    if (pipe == NULL){
        return NULL;
    }

    //pipe结构初始化
    pipe->head = 0;
    pipe->tail = 0;
    pipe->datasize = 0;
    pthread_mutex_init(&pipe->mutex,NULL);
    pthread_cond_init(&pipe->cond,NULL);

    return pipe;
}

int mypipe_register(mypipe_t *ptr,int opmap){
    struct mypipe_st *pipe = ptr;
    
    pthread_mutex_lock(&pipe->mutex);
    if(opmap & PIPE_READER){
        pipe->count_reader++;
    }
    if(opmap & PIPE_WRITER){
        pipe->count_writer++;
    }

    //读写双方不全
    while(pipe->count_reader <= 0 || pipe->count_writer <= 0){
        pthread_cond_wait(&pipe->cond,&pipe->mutex);
    }

    pthread_cond_broadcast(&pipe->cond);//读写双方凑齐
    pthread_mutex_unlock(&pipe->mutex);
    return 0;

}

int mypipe_unregister(mypipe_t *ptr,int opmap){

    struct mypipe_st *pipe = ptr;
    
    pthread_mutex_lock(&pipe->mutex);
    if(opmap & PIPE_READER){
        pipe->count_reader--;
    }
    if(opmap & PIPE_WRITER){
        pipe->count_writer--;
    }
    //唤醒其他管道读写方检查读写者的数量
    pthread_cond_broadcast(&pipe->cond);

    pthread_mutex_unlock(&pipe->mutex);
    return 0;
}

static int mypipe_readbyte_unlocked(struct mypipe_st *pipe,char *data){
    //管道无数据
    if (pipe->datasize <= 0){
        return -1;
    }

    //管道有数据 读取一个现在管道的读端数据,用data保存
    *data = pipe->data[pipe->head];

    pipe->head = (pipe->head++)%PIPESIZE;
    pipe->datasize--;
    return 0;
}

static int mypipe_writebyte_unlocked(struct mypipe_st *pipe,const char *data){
    //管道数据满
    if (pipe->datasize >= PIPESIZE){
        return -1;
    }

    //管道有数据 读取一个现在管道的读端数据,用data保存
    pipe->data[pipe->tail+1] = *data;

    pipe->tail = (pipe->tail++)%PIPESIZE;
    pipe->datasize++;
    return 0;
}

int mypipe_read(mypipe_t *ptr,void *buf,size_t size){
    struct mypipe_st *pipe = ptr;
    
    pthread_mutex_lock(&pipe->mutex);
    
    while(pipe->datasize <= 0 && pipe->count_writer > 0){
        pthread_cond_wait(&pipe->cond,&pipe->mutex);
    }

    //管道空且没有写者
    if (pipe->datasize <= 0 && pipe->count_writer <= 0){
        pthread_mutex_unlock(&pipe->mutex);
        return 0;
    }

    //管道中有数据了
    for (int i = 0;i < size;i++){
        if (mypipe_readbyte_unlocked(pipe,buf+i) < 0){
            break;
        }
    }
    pthread_mutex_unlock(&pipe->mutex);

    return 0;
}

int mypipe_write(mypipe_t *ptr,const void *buf,size_t size){
    struct mypipe_st *pipe = ptr;
    
    pthread_mutex_lock(&pipe->mutex);
    
    while(pipe->datasize >= PIPESIZE && pipe->count_reader > 0){
        pthread_cond_wait(&pipe->cond,&pipe->mutex);
    }

    //管道空且没有读者
    if (pipe->datasize <= 0 && pipe->count_reader <= 0){
        pthread_mutex_unlock(&pipe->mutex);
        return 0;
    }

    //管道中有空间了
    for (int i = 0;i < size;i++){
        if (mypipe_writebyte_unlocked(pipe,buf+i) < 0){
            break;
        }
    }
    pthread_mutex_unlock(&pipe->mutex);

    return 0;

}

int mypipe_destory(mypipe_t *ptr){
    struct mypipe_st *pipe = ptr;
    pthread_mutex_destroy(&pipe->mutex);
    pthread_cond_destroy(&pipe->cond);
    free(pipe);

    return 0;
}

