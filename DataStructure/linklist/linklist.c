#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "linklist.h"

list* list_create(){
  list *head;
  head = malloc(sizeof(*head));
  if (head == NULL){
    return NULL;
  }

  return head;
}

void link_destory(list* me){

}

int list_insert_at(list* me, int i, datatype* data){
  int j = 0;
  list *node = me, *newnode;

  if(i < 0){
    return -EINVAL;
  }
  
  while(j < i && node != NULL){
    node = node->next;
    j++;
  }

  if (!node)


}

int list_order_insert(list* me){}

int list_delete_at(list* me, int i){}

int list_delete(list* me,datatype* data){}

int list_isempty(list* me){}
