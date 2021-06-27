#include "linklist.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void b(datatype data){
  printf("%d ", *(int *)data);
}

int main(){
  int arr[] = {3, 4, 66, 76, 334, 12, 32};
  list *l = NULL;


  for (unsigned long i = 0;i < sizeof(arr)/sizeof(*arr);i++){
    list_insert(&l, arr+i);
  }

  list_show(l, b);

  if ((list_delete(&l)) == -EEXIST){
    exit(1);
  }
  if ((list_delete(&l)) == -EEXIST){
    exit(1);
  }
  if ((list_delete(&l)) == -EEXIST){
    exit(1);
  }

  list_show(l, b);
  list_destory(&l);

  exit(0);

}
