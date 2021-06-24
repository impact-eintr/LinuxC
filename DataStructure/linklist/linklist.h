#ifndef LINKLIST_H_
#define LINKLIST_H_

typedef int datatype;

typedef struct node_st{
  datatype data;
  struct node_st *next;
}list;

list* list_create();
void link_destory(list*);

int list_insert_at(list*, int, datatype*);
int list_order_insert(list*);
int list_delete_at(list*, int);
int list_delete(list *,datatype *);

int list_isempty(list*);

#endif // LINKLIST_H_
