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
