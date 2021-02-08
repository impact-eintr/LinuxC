 
#include<iostream>
#include<string>

using namespace std;

int main()
{
    std::string str("PLease, replace the vowels in this sentence by asterisks.");
    std::string::size_type found = str.find_first_of("aeiou");
    while (found != std::string::npos)
    {
        str[found] = '*';
        found = str.find_first_of("aeiou", found + 1);
    }
    std::cout << str << '\n';
    return 0;
}