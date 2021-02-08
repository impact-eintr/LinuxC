#include <iostream>

int add(int x,int y){
    return (x+y);
}

int sub(int x,int y){
    return (x-y);
}

typedef int (*func)(int ,int);

int op(int x,int y,func ptr){
    int ret = (*ptr)(x,y);
    return ret;
}

int main(){
    int m,n;
    m = op(7,5,add);
    std::cout<<m<<std::endl;
}
