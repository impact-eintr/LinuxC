#include <iostream>
#include <string>
#include <sstream>

int main(){
    std::string mystr;
    float price = 0;
    int quantity = 0;
    std::getline(std::cin,mystr);
    std::stringstream(mystr) >> price;
    std::getline(std::cin,mystr);
    std::stringstream(mystr) >> quantity;
    std::cout << price * quantity<<'\n';
    exit(0);
}
