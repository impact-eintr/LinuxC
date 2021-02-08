#include <iostream>
#include <cstring>

int main(){
    char mystr[20];
    std::strcpy(mystr,"Hello world!");
    std::cout<<mystr<<std::endl;

    std::cout<<"what is your name?\n";
    std::cin.getline(mystr,20);
    std::cout<<"hello"<<mystr<<'\n';

    exit(0);
}
