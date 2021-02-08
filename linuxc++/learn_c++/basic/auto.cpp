//关于auto关键字，在c11中auto是隐式类型定义的关键字
//相当于一个站位符
//（1）语言的类型的强制转换不会修改原来的数据，会另外的开辟一个临时的或者程序中指定的空间来存储强制转换后的值。
//
//（2）C++引用的实现是在符号表中动了手脚，把自己的变量符号对应的内存地址写成了它所引用的那个变量的内存地址了。
//
//（3）C++的cout函数的执行，是根据变量的名称找到对应的内存地址，然后根据变量的类型从内存中抓取相应的数据。
#include <iostream>

void test(int num){
    std::cout << num << std::endl;
}

int main(){
    int a = 10;
    auto *p = &a;//p int
    test(*p);
    auto q = &a;//q int *
    test(*q);
    auto &s = a;//s int
    test(s);
    auto t = s;//t int

    const auto b = a;//const int
    test(b);
    auto c = b;//c
    volatile auto b1 = a;//优化保留
    auto c1 = b;

    const auto &d = a;
    auto &w = d;
    const volatile auto&d1 = a;
    auto &e1 = d;

    exit(0);
}
