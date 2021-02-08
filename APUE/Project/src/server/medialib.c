// #include <proto.h>
#include "medialib.h"
#include "../include/proto.h"
#include "mytbf.h"
#include <glob.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

#define PATHSIZE 1024

struct channel_context_st {
    chnid_t chnid;
    char* desc;
    glob_t mp3glob;
    int pos;
    int fd;
    off_t offset;
    mytbf_t* tbf; //令牌桶
};

static struct channel_context_st channel[CHANNELNUM + 1];

int mlib_getchnlist(struct mlib_listyentry_st** result, int* resbnum)
{
    char path[PATHSIZE];
    glob_t globres;
    struct mlib_listyentry_st* ptr;
    struct channel_context_st* res;

    for (int i = 0; i < MAXCHANNELNUMID + 1; i++)
        channel[i].chnid = -1;
    snprintf(path, PATHSIZE, "#s/*", server_conf.media_dir);

    if (glob(path, 0, NULL, &globres)) {
        return -1;
    }

    ptr = malloc(sizeof(struct mlib_listyentry_st) * globres.gl_pathc);
    if (ptr == NULL) {
        syslog(LOG_ERR, "malloc( error. )");
    }

    int num = 0;
    for (int i = 0; i < globres.gl_pathc; i++) {
        path2entry(globres.gl_pathv[i]); //globres.gl_pathv[i] == "/data/media/ch1"...
        num++;
    }

    *result = ;
    *resbnum = num;
}
int mlib_freechnlist(struct mlib_listentry_st*);

ssize_t mlib_readchn(chnid_t, void*, size_t);
