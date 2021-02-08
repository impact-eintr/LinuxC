#include <iostream>
#include <string>
int test() {
    int choise;
    std::cout << "是否显示结果？(1 / 0)\t" ;
    std::cin >> choise;
    return choise;
}

void who(){

    std::string name;
    std::cout << "who are you?" << std::endl;
    std::getline(std::cin , name);
    std::cout << "Hello "<<name << std::endl;
}

int main(int argc, char **argv) {
    int a = atoi(*(argv + 1));
    int b(atoi(*(argv + 2)));

    who();

    if (test()) {
        auto c = a > b ? a : b;
        std::cout << c << std::endl;

    } else {
        std::cout << "quit" << std::endl;
    }
    return 0;
}
