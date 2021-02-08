#include <iostream>
#include <unistd.h>

int main(){
    pid_t pid;
    pid = getpid();
    std::cout << pid<<std::endl;
    exit(0);
}
