#include <iostream>
#include <fstream>

int main(){
    char data[100];

    std::ofstream outfile;
    outfile.open("test.txt");

    std::cout << "Writeing to the file"<<std::endl;
    std::cout << "Enter your name:";
    std::cin.getline(data,100);

    outfile << data << std::endl;

    std::cout << "Enter your age:";
    std::cin >> data;
    //std::cin.ignore();

    outfile << data << std::endl;

    outfile.close();

    std::ifstream infile;
    infile.open("test.txt");
    std::cout << "Reading ..."<< std::endl;

    for(int i = 0;i < 2;i ++){
        infile >> data;
        std::cout << data<<std::endl;
    }

    infile.close();

    exit(0);
}
