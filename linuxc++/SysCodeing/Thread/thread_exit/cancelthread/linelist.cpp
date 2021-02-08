#include "linelist.h"
template <class D> Status linelist<D>::IsEmpty(){
    if(this->length == 0)
      return TRUE;
    else
      return FALSE;
}

template <class D> Status linelist<D>::InintList(){
    this->length = 0;
    return OK;
}

template <class D> Status linelist<D>::ClearList(){
    this->length = 0;
    return 0;
}

template <class D> int linelist<D>::ListLength(){
    return this->length;
}

template <class D> Status linelist<D>::GetElem(int pos,D* element){
    if(this->length == 0|| pos < 1||pos > this->length )
      return ERROR;
    *element = *((this->data)+pos-1);
    return OK;
}

template <class D> int linelist<D>::LocateElem(D e){
    int i;
    if(this->length == 0){
        return 0;
    }
    for(i = 0;i < this->length;i++){
        if(*((this->data)+i) == e)
          break;
    }
    if(i >= this->length)
      return 0;
    return i+1;
}

template <class D> Status linelist<D>::ListInsert(int pos,D element){
    int k;
    if(this->length == MAXSIZE)
      return ERROR;
    if(pos <1 ||pos >this->length+1 )
      return ERROR;
    if(pos <= this->length){
        for(k = this->length;k>=pos-1;k--){
            this->data[k+1] = this->data[k];
        }
    }
    *((this->data)+pos-1) = element;
    this->length++;
    return OK;
}


/* 初始条件：顺序线性表L已存在，1≤i≤ListLength(L) */
/* 操作结果：删除L的第i个数据元素，并用e返回其值，L的长度减1 */
template <class D> Status linelist<D>::ListDelete(int i, D *e) {
    int k;
    if (this->length == 0) /* 线性表为空 */
      return ERROR;
    if (i < 1 || i >= this->length) /* 删除位置不正确 */
      return ERROR;
    *e = this->data[i-1];
    if (i < this->length) {              /* 如果删除不是最后位置 */
        for (k = i; k < this->length; k++) /* 将删除位置后继元素前移 */
          this->data[k - 1] = this->data[k];
    }
    this->length--;
    return OK;
}

/* 初始条件：顺序线性表L已存在 */
/* 操作结果：依次对L的每个数据元素输出 */
template <class D> Status linelist<D>::ListTraverse() {
    int i;
    for (i = 0; i < this->length; i++)
      visit(this->data[i]);
    printf("\n");
    return OK;
}

template <class D> void linelist<D>::unionL(linelist<D> *Lb) {
    int La_len, Lb_len, i;
    D e;
    La_len = this->ListLength();
    Lb_len = *Lb->ListLength();
    for (i = 1; i <= Lb_len; i++) {
        GetElem(Lb, i, &e);
        if (!this->LocateElem( e))
          this->ListInsert(++La_len, e);
    }
}

