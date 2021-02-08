#include<iostream>
#include<string>

using namespace std;

int main()
{
	cout<<"find_first_of test:"<<endl;
    //测试size_type find_first_of (charT c, size_type pos = 0) const noexcept;
    string str("babccbabcc");
    cout << str.find('a', 0) << endl;//1
    cout << str.find_first_of('a', 0) << endl;//1   str.find_first_of('a', 0)与str.find('a', 0)
    //测试size_type find_first_of (const basic_string& str, size_type pos = 0) const noexcept;
    string str1("bcgjhikl");
    string str2("kghlj");
    cout << str1.find_first_of(str2, 0) << endl;//从str1的第0个字符b开始找，b不与str2中的任意字符匹配；再找c,c不与str2中的任意字符匹配；再找g，
                                                //g与str2中的g匹配，于是停止查找，返回g在str1中的位置2
    //测试size_type find_first_of (const charT* s, size_type pos, size_type n) const;
    cout << str1.find_first_of("kghlj", 0, 20);//2   尽管第3个参数超出了kghlj的长度，但仍能得到正确的结果，可以认为，str1是和"kghlj+乱码"做匹配
    return 0;
}

