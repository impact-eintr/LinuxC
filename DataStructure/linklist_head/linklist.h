#ifndef LINKLIST_H_
#define LINKLIST_H_

typedef void *datatype;

typedef struct node_st{
  datatype data;
  struct node_st *next;
}list;

list* list_create();
void list_destory(list*);

int list_insert_at(list*, int, datatype);
int list_order_insert(list* me, datatype data, int (*)(datatype,datatype));
int list_delete_at(list*, int, datatype);
int list_delete(list* me,datatype data, int (*)(datatype, datatype));

int list_isempty(list*);
void list_show(list *me, void (*)(datatype));

#endif // LINKLIST_H_
