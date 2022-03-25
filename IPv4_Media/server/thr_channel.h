#ifndef THR_CHANNEL_H_
#define THR_CHANNEL_H_

// 普通的频道

#include "medialib.h"

int thr_channel_create(struct mlib_listentry_st *);
int thr_channel_destroy(struct mlib_listentry_st *);
int thr_channel_destroyall(void);

#endif // THR_CHANNEL_H_
