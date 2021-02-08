#ifndef _MYSEM_H_
#define _MYSEM_H_
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef void mysem_t;

mysem_t *mysem_init(int initval);
int mysem_add(mysem_t *, int);
int mysem_sub(mysem_t *, int);
int mysem_destroy(mysem_t *);

#endif
