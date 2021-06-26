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

void list_destory(list* me){
  if (list_isempty(me)){
    return;
  }

  list *node,*next;
  for(node = me->next;next != NULL;node = next){
    next = node->next;
    free(node);
  }
  free(me);

}

int list_insert_at(list* me, int i, datatype data){
  int j = 0;
  list *node = me, *newnode;

  if(i < 0){
    return -EINVAL;
  }
  
  while(j < i && node != NULL){
    node = node->next;
    j++;
  }

  if (node){
    newnode = malloc(sizeof(*newnode));
    if (newnode == NULL){
      return -2;
    }
    newnode->data = data;
    newnode->next = node->next;
    node->next = newnode;
    return 0;

  }else{
    return -3;

  }

}

// 排序插入
int list_order_insert(list* me, datatype data, int (*f)(datatype,datatype)){
  if (me == NULL){
    return EINVAL;
  }

  list *p = me,*q;

  while(p->next && f(p->next->data, data))
    p = p->next;

  q = malloc(sizeof(*q));
  if (q == NULL){
    return -1;
  }

  q->data = data;
  q->next = p->next;
  p->next = q;

  return 0;
}

int list_delete_at(list* me, int i, datatype data){

  if (me == NULL){
    return EINVAL;
  }

  list *node = me;

  for (int j = 1;j < i;j++){
    if (node->next == NULL){
      return -1;
    }
    node = node->next;
  }

  list *tempnode;
  tempnode = node->next;
  node->next = tempnode->next;
  *(void **)data = tempnode->data;
  free(tempnode);
  return 0;

}

int list_delete(list* me,datatype data, int f(datatype, datatype)){
  list *p = me;

  while(p->next && f(p->next->data, data)){
    p = p->next;
  }

  if (!p->next){
    return -1;
  }else{
    list *q;
    q = p->next;
    p->next = q->next;
    free(q);
    return 0;
  }
  return -2;
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

  for (list *node = me->next;node != NULL;node = node->next){
    f(node->data);
  }
  printf("\n");
}
