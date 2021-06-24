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
