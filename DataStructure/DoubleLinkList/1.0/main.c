#include <stdio.h>
#include <stdlib.h>

#include "llist.h"


void print_llist(const void *data){
    int res =  *(int *)data;
    printf("%d ",res);
}

int cmp(const void *a,const void *b){
    return *(int *)a < *(int *)b;
}

int main()
{
    LLIST *headler;
    headler = llist_careate(sizeof(int));
    if (headler == NULL) {
        exit(1);
    }
    
    for (int i = 0;i < 10;i++){
        if (llist_insert(headler,&i,F) < 0){
            llist_destroy(headler);
            exit(1);
        }
    }

    llist_travel(headler,print_llist);
    printf("\n");
    int key = 7;
    void * temp = llist_find(headler,&key,cmp);
    if (temp == NULL){
        printf("查无此项\n");
    }else {
        printf("%d\n",*(int *)temp);
    }

    llist_delete(headler,&key,cmp);
    llist_travel(headler,print_llist);
    printf("\n");

    llist_destroy(headler);

    exit(0);
}

