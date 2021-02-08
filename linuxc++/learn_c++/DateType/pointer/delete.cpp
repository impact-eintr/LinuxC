#include <iostream>
#include <stdlib.h>

int main(){
    char *num = NULL;
    int len;
    int *arr;
    std::cin.getline(num,1024);
    len = atoi(num);
    arr = new int[len];//动态分配内存

    for(int i = 0;i < len;i++){
        std::cin.getline(num,len);
        *(arr+i) = atoi(num);
    }
    delete [] arr;
}
