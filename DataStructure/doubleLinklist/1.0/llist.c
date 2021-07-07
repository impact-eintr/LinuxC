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
