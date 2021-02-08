#ifndef LINELIST_H__
#define LINELIST_H__
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

const int MAXSIZE = 20;
enum {
    ERROR = 0,
    OK = 1,
    FALSE = 0,
    TRUE = 1,
};

typedef int Status;

template <class D> class linelist{
    D data[MAXSIZE];
    Status status;
    int length;
    public:

    Status visit(D data){
        std::cout<<data<<" ";
        return OK;
    }
    Status IsEmpty();
    Status InintList();
    Status ClearList();
    Status GetElem(int,D*);
    int ListLength();
    int LocateElem(D);
    Status ListInsert(int,D );
    Status ListDelete(int,D*);
    Status ListTraverse();
    void unionL(linelist<D>*);
};
#endif
