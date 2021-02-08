#include "linelist.h"

void * thread1(void *arg){
    linelist<int> L;
    L.InintList();
    std::cout<<L.ListLength()<<std::endl;
    for(int i = 1;i <= 10;i++){
        Status res = L.ListInsert(i,i*i);
        if(!res){
            fprintf(stderr,"insert err\n");
            exit(1);
        }
    }
    L.ListTraverse();
    std::cout<<L.ListLength()<<std::endl;
    L.ListInsert(5,0);
    L.ListTraverse();
    std::cout<<L.ListLength()<<std::endl;
    return (void *)0;
}



