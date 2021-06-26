#include "linklist.h"
#include <stdio.h>
#include <stdlib.h>

int a(datatype a, datatype b){
  return *(int *)a < *(int*)b;
}

void b(datatype data){
  printf("%d ", *(int *)data);
}

int main(){
  int arr[] = {3, 4, 66, 76, 334, 12, 32};
  list *l;

  l = list_create();

  for (unsigned long i = 0;i < sizeof(arr)/sizeof(*arr);i++){
    list_order_insert(l, arr+i,a);
  }

  list_show(l, b);

  int *num =  NULL;
  list_delete_at(l, 2, &num);

  printf("%d删除\n", *num);
  list_show(l, b);
  list_destory(l);

  exit(0);

}
