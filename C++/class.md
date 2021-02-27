### c vs cpp
c 的数据是全局的
cpp 的数据和函数可以包含到一个class中,可以是局部的

### c++代码的基本形式
- 类声明 ".h" "hpp"
- 主文件 .cpp .cc
- 标准库 <.h>
## c++ "helloworld"

~~~ cpp
#include <iostream>

int main()
{
    std::cout << "Hello world" << std::endl;
    return 0;
}

~~~

## 头文件的防卫式声明
~~~ cpp
#ifndef PROTE_H__
#define PROTE_H__

//...

#endif
~~~
可能有很多程序要用到你的头文件，这种声明可以防止重复`include`

### cpp类的声明

~~~ cpp
#ifndef COMOLEX_H__
#define COMOLEX_H__

class complex   //class head
{               //class body                
    public:
    complex(double r = 0,double i = 0) :re(r),im(i){}
    complex& operator += (const complex&);  //声明
    double real() const {return re;}        //定义
    double imag() const {return im;}
    ~complex() {}

    private:
    double re,im;//数据

    //friend complex* __doapl {complex*,const complex&};
};


#endif
~~~
我们假设有另一个类与`complex`极为相似 只有数据部分的类型不同 可能是 `int`之类的，这样的类其实没有必要单独声明，c++为我们设计了`模板`这种机制

## cpp 模板

~~~ cpp
#ifndef COMOLEX_H__
#define COMOLEX_H__

template<typename T>//告诉编译器类型未知
class complex   //class head
{               //class body                
    public:
    complex(T r = 0,T i = 0) :re(r),im(i){}
    complex& operator += (const complex&);
    T real() const {return re;}
    T imag() const {return im;}
    ~complex() {}

    private:
    T re,im;//数据

    //friend complex* __doapl {complex*,const complex&};
};


#endif

~~~
使用
~~~ cpp
complex<double> c1(2.3,4.5);
complex<int> c2(2,3);

~~~

## inline（内联）函数

函数如果在class内定义完成就会自动成为inline候选人
~~~ cpp
class complex   //class head
{               //class body                
    public:
    complex(double r = 0,double i = 0) :re(r),im(i){}
    complex& operator += (const complex&);  //声明
    double real() const {return re;}        //定义
    double imag() const {return im;}
    ~complex() {}

    private:
    double re,im;//数据

    //friend complex* __doapl {complex*,const complex&};
};
~~~

给函数加`inline`关键字也可以
~~~ cpp
inline double imag(const& x){
    return x.imag();
}
~~~

## 访问级别
- private 在类外无法访问
~~~ cpp
//不允许
complex c1(2,1);
cout << c1.re;
cout << c1.im;
~~~
- public 在类内可以访问
~~~ cpp
//允许
complex c1(2,1);
cout << c1.real();
cout << c1.imag();
~~~

