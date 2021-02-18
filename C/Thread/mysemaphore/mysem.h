#ifndef MYSEM_H__
#define MYSEM_H__

typedef void mysem_t;

mysem_t *mysem_init(int initval);

int mysem_add(mysem_t *,int);

int mysem_sub(mysem_t *,int);

int mysem_destory(mysem_t *);

#endif
