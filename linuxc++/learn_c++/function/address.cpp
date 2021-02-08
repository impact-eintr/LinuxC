#include <iostream>
#include <ostream>
#include <unistd.h>


void test1(int a,int b,int c);
void test2(int& a,int& b,int& c);
void test3(int* a,int* b,int* c);
int main(){
    int x = 1;
    auto y = 2;
    auto z = 3;
    pid_t pid;
    test1(x,y,z);
    std::cout<<x<<y<<z<<std::endl;

    pid = fork();
    if(pid == 0){
        test3(&x,&y,&z);
        std::cout<<x<<y<<z<<std::endl;
        exit(0);
    }
    sleep(1);
    test2(x,y,z);
    std::cout<<x<<y<<z<<std::endl;
}
void test1(int a,int b,int c){
    a *= 2;
    b *= 2;
    c *= 2;
}

//按地址传参是使函数返回多个值
void test2(int& a,int& b,int& c){
    a *= 2;
    b *= 2;
    c *= 2;
}

void test3(int* a,int* b,int* c){
    *a *= 3;
    *b *= 3;
    *c *= 3;
}


