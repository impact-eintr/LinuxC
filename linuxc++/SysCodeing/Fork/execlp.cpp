#include <unistd.h>
#include <iostream>

int main(){
    execl("mytest",NULL);
    std::cout<<"----------";
    exit(0);
}
