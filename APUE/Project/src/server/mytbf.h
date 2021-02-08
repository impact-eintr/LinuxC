#ifndef MYTBF_H__
#define MYTBF_H__

#define MYTBF_MAX   1024
typedef void mytbf_t;

mytbf_t *mytbf_init(int cps,int burst);

int mytbf_fetchtoken();

int mytbf_returntoken();

int mytbf_destroy();

#endif
