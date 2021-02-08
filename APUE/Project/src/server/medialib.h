#ifndef MEDIALIB_H__
#define MEDIALIB_H__

//#include <proto.h>
#include "../include/proto.h"
struct mlib_listyentry_st{
    chnid_t chnid;
    char *dec;
};

int mlib_getchnlist(struct mlib_listyentry_st **result,int *resnum);
int mlib_freechnlist(struct mlib_listentry_st *);

ssize_t mlib_readchn(chnid_t,void *,size_t);

#endif
