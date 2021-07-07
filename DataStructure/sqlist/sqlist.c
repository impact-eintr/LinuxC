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

int sqlist_destory(sqlist* me){
    free(me);

    return 0;
}

// 数据操作
int sqlist_insert(sqlist* me, int i, datatype* data){
    if (me->last == DATASIZE-1 ) {
        return -1;
    }
    if (i < 0 || i > me->last+1) {
        return -2;
    }

    for (int j = me->last;i <= j;j-- ) {
        me->data[j+1] = me->data[j];
    }
    me->data[i] = *data;
    me->last++;

    return 0;
}

int sqlist_delete(sqlist* me, int i){
    if (i < 0 || i > me->last){
        return -1;
    }
    for (int j = i;j < me->last;j++) {
        *(me->data+j) = *(me->data+j+1);
    }
    me->last--;
    return 0;
}

int sqlist_find(sqlist* me, datatype* data){

    if (sqlist_isempty(me) == 0){
        return -1;
    }
    for (int i = 0;i <= me->last;i++){
        if (*(me->data+i) == *data) {
            return i;
        }
    }

    return - -1;
}

int sqlist_union(sqlist* ptr1, sqlist* ptr2) {
    return 0;
}

// 工具方法
int sqlist_isempty(sqlist* me){
    if (me->last == -1){
        return 0;
    }else{
        return -1;
    }
}

int sqlist_setempty(sqlist* me){
    return 0;
}

int sqlist_getnum(sqlist* me){
    return me->last+1;
}

void sqlist_show(sqlist *me){
    if (me->last == -1) {
        return;
    }

    for (int i = 0;i <= me->last;i++) {
        printf("%d ", *(me->data+i));
    }
    printf("\n");
}

