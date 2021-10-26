# 线性表

``` c++
#ifndef SQLIST_H_
#define SQLIST_H_

#define DATASIZE 1024

typedef int datatype;

typedef struct 
{
  datatype data[DATASIZE];
  int last;
}sqlist;

sqlist *sqlist_create();
void sqlist_createone(sqlist **);

// 数据操作
int sqlist_insert(sqlist*, int, datatype*);
int sqlist_delete(sqlist*, int);
int sqlist_find(sqlist*, datatype*);

// 工具方法
int sqlist_isempty(sqlist*);
int sqlist_setempty(sqlist*);
int sqlist_getnum(sqlist*);
void sqlist_show(sqlist*);

int sqlist_destory(sqlist*);

#endif // SQLIST_H_

```

``` c++
#include <stdio.h>
#include <stdlib.h>
#include "sqlist.h"

sqlist *sqlist_create(){
    sqlist *me;
    me = malloc(sizeof(*me));
    if (me == NULL) {
        return NULL;
    }
    me->last = -1;
    return me;
}
void sqlist_createone(sqlist **ptr){
    *ptr = malloc(sizeof(**ptr));
    if (*ptr == NULL) {
        return;
    }

    (*ptr)->last = -1;
    return;
}

int sqlist_destory(sqlist* me){
    free(me);

    return 0;
}

// 数据操作
int sqlist_insert(sqlist* me, int i, datatype* data){
    if (me->last == DATASIZE-1 ) {
        return -1;
    }
    if (i < 0 || i > me->last+1) {
        return -2;
    }

    for (int j = me->last;i <= j;j-- ) {
        me->data[j+1] = me->data[j];
    }
    me->data[i] = *data;
    me->last++;

    return 0;
}

int sqlist_delete(sqlist* me, int i){
    if (i < 0 || i > me->last){
        return -1;
    }
    for (int j = i;j < me->last;j++) {
        *(me->data+j) = *(me->data+j+1);
    }
    me->last--;
    return 0;
}

int sqlist_find(sqlist* me, datatype* data){

    if (sqlist_isempty(me) == 0){
        return -1;
    }
    for (int i = 0;i <= me->last;i++){
        if (*(me->data+i) == *data) {
            return i;
        }
    }

    return - -1;
}

int sqlist_union(sqlist* ptr1, sqlist* ptr2) {
    return 0;
}

// 工具方法
int sqlist_isempty(sqlist* me){
    if (me->last == -1){
        return 0;
    }else{
        return -1;
    }
}

int sqlist_setempty(sqlist* me){
    return 0;
}

int sqlist_getnum(sqlist* me){
    return me->last+1;
}

void sqlist_show(sqlist *me){
    if (me->last == -1) {
        return;
    }

    for (int i = 0;i <= me->last;i++) {
        printf("%d ", *(me->data+i));
    }
    printf("\n");
}


```


``` c++
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

```

# 链表

## 单向有头 

``` c++
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


```


``` c++
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

```


``` c++
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

```


## 单向无头

``` c++
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

```



``` c++
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

```


``` c++
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

```

### 单向链表的应用

``` c++
#include <stdio.h>
#include <stdlib.h>

struct node_st{
  int coef; // 系数
  int exp;  // 指数
  struct node_st *next;
};

struct node_st *poly_create(int a[][2], int size){
  struct node_st *me;
  struct node_st *temp;
  struct node_st *cur;

  me = malloc(sizeof(*me));
  if (me == NULL){
    return NULL;;
  }
  me->next = NULL;
  me->exp = -1;
  cur = me;

  for(int i = 0;i < size;i++){
    temp = malloc(sizeof(*temp));
    // 构建失败的话 要进行内存释放
    if (temp == NULL){
      cur = me;
      struct node_st *q;
      for (int j = 0;j < i;j++){
        q = cur->next;
        free(cur);
        cur = q;
      }
    }
    // 构建新节点
    temp->coef = *(*(a+i));
    temp->exp = *(*(a+i)+1);
    temp->next = NULL;
    // 插入正确的位置
    cur = me;
    while(cur->next && cur->next->exp < temp->exp){
      cur = cur->next;
    }
    temp->next = cur->next;
    cur->next = temp;
  }

  return me;

}

void poly_union(p1, p2){

}

void poly_destory(struct node_st* p){
  free(p);
}

void poly_show(struct node_st *me){
  struct node_st *temp;
  for (temp = me->next;temp != NULL;temp = temp->next){
    printf("%d*X^%d ", temp->coef, temp->exp);
  }
  printf("\n");

}

int main(){
  int a[][2] = {{5,0},{8,8},{2,1},{3,16}};
  int b[][2] = {{6,1},{16,6},{-8,8}};
  struct node_st *p1,*p2;

  p1 = poly_create(a, 4);
  p2 = poly_create(b, 3);

  if (p1 == NULL || p2 == NULL){
    exit(1);
  }

  poly_show(p1);
  poly_show(p2);

  poly_destory(p1);
  poly_destory(p2);

}

```

## 双向链表

### 
``` c++
#ifndef LLIST_H__
#define LLIST_H__
enum{
    F = 1,
    B = 2,
};

//普通节点
struct llist_node_st{
    void *data;
    struct llist_node_st *prev;
    struct llist_node_st *next;
};
//头节点
typedef struct {
    int size;
    struct llist_node_st head;
} LLIST; //LLIST就是一个双向链表的头节点类型，对于链表的操作都是用head来进行的

//传入 每个数据节点的数据类型大小
LLIST *llist_careate(int size);
//传入 一个已经创好的链表的头节点，插入的数据，插入的模式
int llist_insert(LLIST *,const void *data,int mode);
//传入
void *llist_find(LLIST *head,const void* ,int (*func)(const void*,const void*));
//
int llist_delete(LLIST *head,const void* ,int (*func)(const void*,const void*));
//
int llist_fetch(LLIST *head,const void* ,int (*func)(const void*,const void*),void *);
//传入 一个已经创建好的链表头节点
void llist_travel(LLIST* head,void (*func)(const void*));
void llist_destroy(LLIST *);

#endif

```


``` c++
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "llist.h"


//传入 每个数据节点的数据类型大小
LLIST *llist_careate(int size){
    LLIST *new;
    new = malloc(sizeof(*new));
    if (new == NULL){
        return NULL;
    }

    new->size = size;
    new->head.data = NULL;
    new->head.prev = &new->head;
    new->head.next = &new->head;
    
    return new;
}
//传入 一个已经创好的链表的头节点，插入的数据，插入的模式
int llist_insert(LLIST *head,const void *data,int mode){
    struct llist_node_st *newnode;
    newnode = malloc(sizeof(*newnode));
    if (newnode == NULL)
      return -1;

    newnode->data = malloc(head->size);
    if (newnode->data == NULL){
        return -2;
    }
    memcpy(newnode->data,data,head->size);

    switch (mode) {
        case F:
            newnode->prev = &head->head;
            newnode->next = head->head.next;
            break;
        case B:
            newnode->prev = head->head.prev;
            newnode->next = &head->head;
            break;
        default:
            return -3;
    }

    newnode->prev->next = newnode;
    newnode->next->prev = newnode;

    return 0;

}
//传入 一个已经创建好的链表头节点,一个辅助遍历函数
void llist_travel(LLIST* head,void (*func)(const void *)){
    struct llist_node_st *cur,*next;
    for (cur = head->head.next;cur != &head->head;cur = next) {
        func(cur->data);
        next = cur->next;
    }
}

//辅助函数
static struct llist_node_st *find_(LLIST *head,const void *key,int (*func)(const void *,const void *)){
    struct llist_node_st *cur;
    for (cur = head->head.next;cur != &head->head;cur = cur->next){
        if (func(key,cur->data) == 0){
            return cur;
        }
    }
    return &head->head;
}

void *llist_find(LLIST *head,const void* key,int (*func)(const void*,const void*)){
    return find_(head,key,func)->data;
}

//
int llist_delete(LLIST *head,const void* key,int (*func)(const void*,const void*)){
    struct llist_node_st *node;
    node = find_(head,key,func);
    if (node == &head->head){
        return -1;
    }else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        free(node->data);
        free(node);
        return 0;
    }
}
//
int llist_fetch(LLIST *head,const void* key,int (*func)(const void*,const void*),void *data){
    struct llist_node_st *node;
    node = find_(head,key,func);
    if (node == &head->head){
        return -1;
    }else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        data = node->data;
        free(node->data);
        free(node);
        return 0;
    }
}

void llist_destroy(LLIST *head) {
    struct llist_node_st *cur,*next;

    for (cur = head->head.next;cur != &head->head;cur = next) {
        next = cur->next;
        free(cur->data);
        free(cur);
    }
    free(head);
}

```


``` c++
#include <stdio.h>
#include <stdlib.h>

#include "llist.h"


void print_llist(const void *data){
    int res =  *(int *)data;
    printf("%d ",res);
}

int cmp(const void *a,const void *b){
    return *(int *)a < *(int *)b;
}

int main()
{
    LLIST *headler;
    headler = llist_careate(sizeof(int));
    if (headler == NULL) {
        exit(1);
    }
    
    for (int i = 0;i < 10;i++){
        if (llist_insert(headler,&i,F) < 0){
            llist_destroy(headler);
            exit(1);
        }
    }

    llist_travel(headler,print_llist);
    printf("\n");
    int key = 7;
    void * temp = llist_find(headler,&key,cmp);
    if (temp == NULL){
        printf("查无此项\n");
    }else {
        printf("%d\n",*(int *)temp);
    }

    llist_delete(headler,&key,cmp);
    llist_travel(headler,print_llist);
    printf("\n");

    llist_destroy(headler);

    exit(0);
}


```


### 
``` c++

```


``` c++

```


``` c++

```


### 
``` c++

```


``` c++

```


``` c++

```


### 
``` c++

```


``` c++

```


``` c++

```


### 内核双向链表赏析

# 栈和队列

## 栈
``` c++

```


``` c++

```


``` c++

```


## 队列
``` c++

```


``` c++

```


``` c++

```


# 树

## 二叉树
``` c++

```


``` c++

```


``` c++

```



## 平衡二叉树

``` c++

```


``` c++

```


``` c++

```



## 树转广义表

``` c++

```


``` c++

```


``` c++

```


## 二叉搜索树

``` c++

```


``` c++

```


``` c++

```



