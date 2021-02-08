#include<iostream>
#include<string>

using namespace std;

int main()
{
    //测试size_type find_first_not_of (const charT* s, size_type pos = 0) const;
    string str("abcdefg");
    cout << str.find_first_not_of("kiajbvehfgmlc", 0) << endl;//3   从源串str的位置0(a)开始查找，目标串中有a（匹配），再找b，b匹配,再找c，c匹配，
//    再找d，目标串中没有d（不匹配），停止查找，返回d在str中的位置3
    return 0;
}

