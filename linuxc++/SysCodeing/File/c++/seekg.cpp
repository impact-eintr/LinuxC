#include <fstream>
#include <iostream>

const char *filename = "test.txt";
int main() {
    long l, m;
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    l = file.tellg();
    file.seekg(0, std::ios::end);
    m = file.tellg();
    file.close();
    std::cout << "size of " << filename;
    std::cout << " is " << (m - l) << " bytes.\n";
    exit(0);
}
