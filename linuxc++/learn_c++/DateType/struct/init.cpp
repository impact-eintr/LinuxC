#include <cstring>
#include <stdlib.h>
#include <iostream>

const int N = 5;

struct init_st{
    char *title;
    int year;
}book[N];

typedef struct init_st init_t;

void bookinit(int Num,int len,const char *name,int year){
    book[Num].title = new char[len];
    strcpy(book[Num].title,name);
    book[Num].year = year;

}

void exit_book(){
    for(int i = 0;i< N;i++)
      delete [] book[i].title;
}
int main(){
    for(int i = 0;i < 5;i++){
        const char *bookname = "BigData:How it change our world?";
        bookinit(50,bookname,1999);
    }
    atexit(exit_book);

    struct init_st book2;



}
