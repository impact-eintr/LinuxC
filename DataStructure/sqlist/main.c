#include <stdlib.h>
#include <stdio.h>
#include "sqlist.h"

int main() {
    sqlist *list;
    list = sqlist_create();
    // sqlist_createone(&list);
    int err = -1;
    if (list == NULL) {
        perror("list init faild");
        exit(1);
    }

    int arr[] = {21, 34, 54, 98, 56, 1, 3, 5};
    for (int i = 0;i < sizeof(arr)/sizeof(*arr);i++) {
        if ((err = sqlist_insert(list, i, arr+i)) != 0){
            if (err == -1){
                perror("数组为空");
            }else if (err == -2){
                perror("插入失败");
            }else{
                perror("未知错误");
            }
        }
    }

    sqlist_show(list);
    sqlist_delete(list, 1);
    sqlist_show(list);

    printf("顺序表长度: %d\n", sqlist_getnum(list));
    int num = 5;
    printf("%d的位置: %d\n",num ,sqlist_find(list, &num));

    sqlist_destory(list);

    exit(0);
}
