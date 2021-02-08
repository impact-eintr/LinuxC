#include <string.h>
#include <iostream>
int main(int argc,char **argv){
    int i;
    std::cout<<"argc= "<<argc<<std::endl;

    for(i = 0;i < argc;i++){
        std::cout<<argv[i]<<std::endl;
    }
}
