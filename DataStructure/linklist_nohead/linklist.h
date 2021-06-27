#ifndef LINKLIST_H_
#define LINKLIST_H_

typedef void *datatype;

typedef struct node_st{
  datatype data;
  struct node_st *next;
}list;

list* list_create();
void list_destory(list**);

// 首部插入
int list_insert(list** me, datatype data);
int list_delete(list** me);

int list_isempty(list*);
void list_show(list *me, void (*)(datatype));

#endif // LINKLIST_H_
