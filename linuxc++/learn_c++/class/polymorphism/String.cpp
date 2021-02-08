#include "String.h"
//传统写法完成String深拷贝
String::String(const char* str)
        : _str(new char[strlen(str) + 1])
{
    strcpy(_str, str);
}

String::String(const String& s) // 必须用引用否则会无限的递归
        : _str(new char[strlen(s._str) +1]) // 开辟s._str大小的空间，+1是因为‘/0’
{
    strcpy(_str, s._str);
}

String& String::operator=(const String& s)
{
    if (this != &s) // 判断是否相同
    {
        if(_str)
          delete[] _str; // _str不为空，则需要先释放空间
        _str = new char[strlen(s._str) + 1];
        strcpy(_str, s._str);
    }
    return *this;
}

String::~String()
{
    if (NULL != _str)
    {
        delete[] _str;
    }
}

////现代写法完成String深拷贝
//String::String(const char* str)
//        : _str(new char[strlen(str) + 1])
//{
//    strcpy(_str, str);
//}
//
//String::String(const String& s)
//{
//    String tmp(s._str); // 用s._str定义一个tmp对象
//    std::swap(_str, tmp._str); // 将指针进行交换
//}
//
//String String::operator=(String s) // 形参调用构造函数
//{
//    std::swap(_str, s._str);
//    return *this;
