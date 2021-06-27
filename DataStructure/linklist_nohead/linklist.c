#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "linklist.h"

list* list_create(){
  list *head;
  head = malloc(sizeof(*head));
  if (head == NULL){
    return NULL;
  }

  return head;
}

void list_destory(list** me){
  if (list_isempty(*me)){
    return;
  }

  list *node,*next;
  for(node = *me;next != NULL;node = next){
    next = node->next;
    free(node);
  }
  free(*me);

}

int list_insert(list** me, datatype data){
  list *newnode;

  newnode = malloc(sizeof(*newnode));
  if (newnode == NULL){
    return ENOMEM;
  }



  newnode->data = data;
  newnode->next = *me;

  *me = newnode;

  return 0;

}

// 首部删除
int list_delete(list** me){
  list *p = *me;

  if (!p){
    return -EEXIST;
  }

  *me = p->next;
  free(p);
  return 0;

}

int list_isempty(list* me){
  if (me == NULL){
    return 1;
  }
  return 0;
}

void list_show(list *me, void f(datatype)){
  if (list_isempty(me)){
    return;
  }

  for (list *node = me;node != NULL;node = node->next){
    f(node->data);
  }
  printf("\n");
}
