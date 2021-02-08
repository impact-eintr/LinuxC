#ifndef THR_LIST_H__
#define THR_LIST_H__

#include "../include/proto.h"
//#include <proto.h>
#include "medialib.h"

//创建节目单线程
int thr_list_create(struct mlib_listyentry_st*,int);

int thr_list_destroy(void);


#endif
