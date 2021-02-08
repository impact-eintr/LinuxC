#ifndef THR_CHANNEL_H__
#define THR_CHANNEL_H__

#include "../include/proto.h"
//#include <proto.h>
#include "medialib.h"


int thr_channel_create(struct mlib_listyentry_st *);

int thr_channel_destroy(struct mlib_listyentry_st *);

int thr_channel_destroyall(void);

#endif
