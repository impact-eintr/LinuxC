#include<iostream>
#include<string>

using namespace std;

int main()
{
    cout<<"find test"<<endl;
    //测试size_type find (charT c, size_type pos = 0) const noexcept;
    string st1("babbabab");
    cout << st1.find('a') << endl;//1   由原型知，若省略第2个参数，则默认从位置0（即第1个字符）起开始查找
    cout << st1.find('a', 0) << endl;//1
    cout << st1.find('a', 1) << endl;//1   
    cout << st1.find('a', 2) << endl;//4   在st1中，从位置2（b，包括位置2）开始，查找字符a，返回首次匹配的位置，若匹配失败，返回npos
    cout << st1.rfind('a',7) << endl;//6   关于rfind，后面讲述
    cout << st1.find('c', 0) << endl;//4294967295
    cout << (st1.find('c', 0) == -1) << endl;//1
    cout << (st1.find('c', 0) == 4294967295) << endl;//1   两句均输出1，原因是计算机中-1和4294967295都表示为32个1（二进制）
    cout << st1.find('a', 100) << endl;//4294967295   当查找的起始位置超出字符串长度时，按查找失败处理，返回npos
    //测试size_type find (const basic_string& str, size_type pos = 0) const noexcept;
    string st2("aabcbcabcbabcc");
    string str1("abc");
    cout << st2.find(str1, 2) << endl;//6   从st2的位置2（b）开始匹配，返回第一次成功匹配时匹配的串（abc）的首字符在st2中的位置，失败返回npos
    //测试size_type find (const charT* s, size_type pos = 0) const;
    cout << st2.find("abc", 2) << endl; //6   同上，只不过参数不是string而是char*
    //测试size_type find (const charT* s, size_type pos, size_type n) const;
    cout << st2.find("abcdefg", 2, 3) << endl;//6   取abcdefg得前3个字符（abc）参与匹配，相当于st2.find("abc", 2)
    cout << st2.find("abcbc", 0, 5) << endl;//1   相当于st2.find("abcbc", 0)
    cout << st2.find("abcbc", 0, 6) << endl;//4294967295   第3个参数超出第1个参数的长度时，返回npos
    return 0;
}