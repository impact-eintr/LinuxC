//找出字符串str中所有的"abc"（输出位置），若未找到，输出"not find!"
#include<iostream>
#include<string>

using namespace std;

int main()
{
    string str("babccbabcaabcccbabccabcabcabbabcc");
    int num = 0;
    size_t fi = str.find("abc", 0);    
    while (fi!=str.npos)
    {
        cout << fi << "   ";
        num++;
        fi = str.find("abc", fi + 1);
    }
    if (0 == num)
        cout << "not find!";
    cout << endl;
    return 0;
}