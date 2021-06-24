#include <stdlib.h>
#include <stdio.h>
#include "sqlist.h"

int main() {
    sqlist *list;
    list = sqlist_create();
    // sqlist_createone(&list);
    if (list == NULL) {
        perror("list init faild");
        exit(1);
    }

    sqlist_insert(list, 0, 20);
    sqlist_show(list);

    sqlist_destory(list);

    exit(0);
}