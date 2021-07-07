#ifndef MYTBF_H__
#define MYTBF_H__

#define MYTBF_MAX 1024

typedef void mytbf_t;

mytbf_t *mytbf_init(int cps,int burst);

//获取token
int mytbf_fetchtoken(mytbf_t *,int);
//归还token
int mytbf_returntoken(mytbf_t *,int);

int mytbf_destroy(mytbf_t *);

#endif
