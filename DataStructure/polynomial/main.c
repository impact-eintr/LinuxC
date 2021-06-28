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
