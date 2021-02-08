#ifndef _MYTBF_H_H
#define _MYTBF_H_H

#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#include <unistd.h>
#define MYTBF_MAX	1024

typedef void mytbf_t;

mytbf_t * mytbf_init(int cps,int rst);

int mytbf_fetchtoken(mytbf_t *,int );

int mytbf_returntoken(mytbf_t *,int );

int mytbf_destrory(mytbf_t * );

#endif
