#include <stdio.h>
#include <stdlib.h>
#include "sqlist.h"

sqlist *sqlist_create(){
    sqlist *me;
    me = malloc(sizeof(*me));
    if (me == NULL) {
        return NULL;
    }
    me->last = -1;
    return me;
}
void sqlist_createone(sqlist **ptr){
    *ptr = malloc(sizeof(**ptr));
    if (*ptr == NULL) {
        return;
    }

    (*ptr)->last = -1;
    return;
}

// 数据操作
int sqlist_insert(sqlist* me, int i, datatype* data){
    if (me->last == DATASIZE-1 ) {
        return -1;
    }
    if (i < 0 || i > me->last+1) {
        return -2;
    }

    for (int j = me->last; )
}
int sqlist_delete(sqlist*, int){}
int sqlist_find(sqlist*, int ,datatype*){}
int sqlist_union(sqlist*, sqlist*);

// 工具方法
int sqlist_isempty(sqlist*){}
int sqlist_setempty(sqlist*){}
int sqlist_getnum(sqlist*){}
int sqlist_show(sqlist*){}

int sqlist_destory(sqlist*){}
