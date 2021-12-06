# const那些事

## 1.const含义
常类型是指使用类型修饰符const说明的类型，常类型的变量或对象的值是不能被更新的。

## 2.const作用
- 可以定义常量
``` c++
const int a = 100;
```

- 类型检查
const常量与#define宏定义常量的区别：~~const常量具有类型，编译器可以进行安全检查；#define宏定义没有数据类型，只是简单的字符串替换，不能进行安全检查~~。

- 防止修改，起保护作用，增加程序健壮性
``` c++
void f(const int i) {
    i++; // error
}
```

- 可以节省空间，避免不必要的内存分配
const定义常量从汇编的角度来看，只是给出了对应的内存地址，而不是像#define一样给出的是立即数，所以，const定义的常量在程序运行过程中只有一份拷贝，而#define定义的常量在内存中有若干个拷贝。

## 3.const对象默认为文件局部变量
**注意：非const变量默认为extern。要使const变量能够在其他文件中访问，必须在文件中显式地指定它为extern。**

``` c++
// file1.cpp
int ext
// file2.cpp
#include<iostream>
/**
 * by eintr
 * compile: g++ -o file file2.cpp file1.cpp
 * execute: ./file
 */
extern int ext;

int main(){
    std::cout<<(ext+10)<<std::endl;
}

```

**小结：可以发现未被const修饰的变量不需要extern显式声明！而const常量需要显式声明extern，并且需要做初始化！因为常量在定义后就不能被修改，所以定义时必须初始化。**

## 4.定义常量
``` c++
const int b = 10;
b = 0; // error b为常量，不可更改
const string s "helloworld";
const int i, j = 0; // i为常量，必须进行初始化！(因为常量在定义后就不能被修改，所以定义时必须初始化。)
```


## 5.指针与const

``` c++
const char * a; //指向const对象的指针或者说指向常量的指针。
char const * a; //同上
char * const a; //指向类型对象的const指针。或者说常指针、const指针。
const char * const a; //指向const对象的const指针。
```

``` c++
#include <iostream>

using namespace std;

int main() {
  int i = 12;
  int j = 10;

  const int *ip = &i;
  ip = &j;

  int const *ip_ = &i;
  ip_ = &j;

  int * const iq = &i;
  *iq = j;

  const int * const ipq = &i;
}

```

小结：如果const位于*的左侧，则const就是用来修饰指针所指向的变量，即指针指向为常量；如果const位于*的右侧，const就是修饰指针本身，即指针本身是常量。

具体使用如下：

- （1）指向常量的指针
``` c++
const int *ptr;
*ptr = 10; //error

```

ptr是一个指向int类型const对象的指针，const定义的是int类型，也就是ptr所指向的对象类型，而不是ptr本身，所以ptr可以不用赋初始值。但是不能通过ptr去修改所指对象的值。

除此之外，也不能使用void*指针保存const对象的地址，必须使用const void*类型的指针保存const对象的地址。

``` c++
const int p = 10;
const void * vp = &p;
void *vp = &p; //error
```

另外一个重点是：允许把非const对象的地址赋给指向const对象的指针。

将非const对象的地址赋给const对象的指针:

``` c++
const int *ptr;
int val = 3;
ptr = &val; //ok
```

我们不能通过ptr指针来修改val的值，即使它指向的是非const对象!

我们不能使用指向const对象的指针修改基础对象，然而如果该指针指向了非const对象，可用其他方式修改其所指的对象。可以修改const指针所指向的值的，但是不能通过const对象指针来进行而已！如下修改：

``` c++
int *ptr1 = &val;
*ptr1=4;
cout<<*ptr<<endl;
```

小结：对于指向常量的指针，不能通过指针来修改对象的值。
也不能使用void`*`指针保存const对象的地址，必须使用const void`*`类型的指针保存const对象的地址。
允许把非const对象的地址赋值给const对象的指针，如果要修改指针所指向的对象值，必须通过其他方式修改，不能直接通过当前指针直接修改。

- （2）常指针

const指针必须进行初始化，且const指针的值不能修改。

``` c++
#include<iostream>
using namespace std;
int main(){

    int num=0;
    int * const ptr=&num; //const指针必须初始化！且const指针的值不能修改
    int * t = &num;
    *t = 1;
    cout<<*ptr<<endl;
}
```

上述修改ptr指针所指向的值，可以通过非const指针来修改。

最后，当把一个const常量的地址赋值给ptr时候，由于ptr指向的是一个变量，而不是const常量，所以会报错，出现：const int* -> int *错误！

``` c++
#include<iostream>
using namespace std;
int main(){
    const int num=0;
    int * const ptr=&num; //error! const int* -> int*
    cout<<*ptr<<endl;
}
```

上述若改为 const int *ptr或者改为const int *const ptr，都可以正常！

- （3）指向常量的常指针

理解完前两种情况，下面这个情况就比较好理解了：

``` c++
const int p = 3;
const int * const ptr = &p; 
```

ptr是一个const指针，然后指向了一个int 类型的const对象。

## 6.函数中使用const

> cost修饰函数返回值

（1）const int

``` c++
const int func1();
```

这个本身无意义，因为参数返回本身就是赋值给其他的变量！

（2）const int*

``` c++
const int* func2();
```

指针指向的内容不变。

（3）int *const

``` c++
int *const func2();
```

指针本身不可变。

> const修饰函数参数

- （1）传递过来的参数及指针本身在函数内不可变，无意义！

``` c++
void func(const int var); // 传递过来的参数不可变
void func(int *const var); // 指针本身不可变
```

表明参数在函数体内不能被修改，但此处没有任何意义，var本身就是形参，在函数内不会改变。包括传入的形参是指针也是一样。

输入参数采用“值传递”，由于函数将自动产生临时变量用于复制该参数，该输入参数本来就无需保护，所以不要加const 修饰。

- （2）参数指针所指内容为常量不可变

``` c++
void StringCopy(char *dst, const char *src);
```

其中src 是输入参数，dst 是输出参数。给src加上const修饰后，如果函数体内的语句试图改动src的内容，编译器将指出错误。这就是加了const的作用之一。

- （3）参数为引用，为了增加效率同时防止修改。

``` c++
void func(const A &a)
```

对于非内部数据类型的参数而言，象void func(A a) 这样声明的函数注定效率比较低。因为函数体内将产生A 类型的临时对象用于复制参数a，而临时对象的构造、复制、析构过程都将消耗时间。

为了提高效率，可以将函数声明改为void func(A &a)，因为“引用传递”仅借用一下参数的别名而已，不需要产生临时对象。但是函数void func(A &a) 存
在一个缺点：

“引用传递”有可能改变参数a，这是我们不期望的。解决这个问题很容易，加const修饰即可，因此函数最终成为

``` c++
void func(const A &a)。
```

以此类推，是否应将void func(int x) 改写为void func(const int &x)，以便提高效率？完全没有必要，因为内部数据类型的参数不存在构造、析构的过程，而复制也非常快，“值传递”和“引用传递”的效率几乎相当。

**小结：对于非内部数据类型的输入参数，应该将“值传递”的方式改为“const 引用传递”，目的是提高效率。例如将void func(A a) 改为void func(const A &a)。
对于内部数据类型的输入参数，不要将“值传递”的方式改为“const 引用传递”。否则既达不到提高效率的目的，又降低了函数的可理解性。例如void func(int x) 不应该改为void func(const int &x)。**

## 7.类中使用const

在一个类中，任何不会修改数据成员的函数都应该声明为const类型。如果在编写const成员函数时，不慎修改数据成员，或者调用了其它非const成员函数，编译器将指出错误，这无疑会提高程序的健壮性。使用const关字进行说明的成员函数，称为常成员函数。只有常成员函数才有资格操作常量或常对象，没有使用const关键字声明的成员函数不能用来操作常对象。

对于类中的const成员变量必须通过初始化列表进行初始化，如下所示：

``` c++
class Apple
{
private:
    int people[100];
public:
    Apple(int i); 
    const int apple_number;
};

Apple::Apple(int i):apple_number(i)
{

}
```

**const对象只能访问const成员函数,而非const对象可以访问任意的成员函数,包括const成员函数.**

``` c++
//apple.cpp
class Apple
{
private:
    int people[100];
public:
    Apple(int i); 
    const int apple_number;
    void take(int num) const;
    int add(int num);
    int add(int num) const;
    int getCount() const;

};
//main.cpp
#include<iostream>
#include"apple.cpp"
using namespace std;

Apple::Apple(int i):apple_number(i)
{

}
int Apple::add(int num){
    take(num);
}
int Apple::add(int num) const{
    take(num);
}
void Apple::take(int num) const
{
    cout<<"take func "<<num<<endl;
}
int Apple::getCount() const
{
    take(1);
//    add(); //error
    return apple_number;
}
int main(){
    Apple a(2);
    cout<<a.getCount()<<endl;
    a.add(10);
    const Apple b(3);
    b.add(100);
    return 0;
}
//编译： g++ -o main main.cpp apple.cpp
//结果
take func 1
2
take func 10
take func 100
```

|                                                      | const object(data members 不得变动) | non-const object(data members可以变动 |
| :--------------------------------------------------: | :---------------------------------: | :-----------------------------------: |
|    const member functions(保证不修改data members)    |                  T                  |                   T                   |
| non-const member functions(不保证不修改data members) |                  F                  |                   T                   |

**当成员函数的const和non-const版本同时存在，const object只能调用 const 版本 non-const object只能调用 non-const版本**

# static 那些事
当与不同类型一起使用时，Static关键字具有不同的含义。我们可以使用static关键字：

静态变量： 函数中的变量，类中的变量

静态类的成员： 类对象和类中的函数

现在让我们详细看一下静态的这些用法：

## 静态变量

### 函数中的静态变量

当变量声明为static时，空间将在程序的生命周期内分配。**即使多次调用该函数，静态变量的空间也只分配一次，前一次调用中的变量值通过下一次函数调用传递**。这对于在C / C ++或需要存储先前函数状态的任何其他应用程序非常有用。

``` c++
#include <iostream> 
#include <string> 
using namespace std; 

void demo() 
{ 
    // static variable 
    static int count = 0; 
    cout << count << " "; 

    // value is updated and 
    // will be carried to next 
    // function calls 
    count++; 
} 

int main() 
{ 
    for (int i=0; i<5; i++)
        demo(); 
    return 0; 
} 
```

输出：

``` c++
0 1 2 3 4 
```

您可以在上面的程序中看到变量count被声明为static。因此，它的值通过函数调用来传递。每次调用函数时，都不会对变量计数进行初始化。

### 类中的静态变量

由于声明为static的变量只被初始化一次，因为它们在单独的静态存储中分配了空间，因此类中的静态变量由对象共享。对于不同的对象，不能有相同静态变量的多个副本。也是因为这个原因，静态变量不能使用构造函数初始化。

``` c++
#include <iostream>
using namespace std;

class apple {
public:
  static int i;

  apple(){
      // do nothing
  };
};

int apple::i = 1;

int main() {
  apple obj1;
  apple obj2;

  cout << apple::i << endl;

  obj1.i = 2;
  cout << obj1.i << endl;

  obj2.i = 3;
  cout << obj2.i << endl;
}

```


## 静态类的成员变量

### 类对象为静态
就像变量一样，对象也在声明为static时具有范围，直到程序的生命周期。

考虑以下程序，其中对象是非静态的。

``` c++
#include<iostream> 
using namespace std; 

class Apple 
{ 
    int i; 
    public: 
        Apple() 
        { 
            i = 0; 
            cout << "Inside Constructor\n"; 
        } 
        ~Apple() 
        { 
            cout << "Inside Destructor\n"; 
        } 
}; 

int main() 
{ 
    int x = 0; 
    if (x==0) 
    { 
        Apple obj; 
    } 
    cout << "End of main\n"; 
}
```

 
输出：

``` c++
Inside Constructor
Inside Destructor
End of main
```

在上面的程序中，对象在if块内声明为非静态。因此，变量的范围仅在if块内。因此，当创建对象时，将调用构造函数，并且在if块的控制权越过析构函数的同时调用，因为对象的范围仅在声明它的if块内。 如果我们将对象声明为静态，现在让我们看看输出的变化。

``` c++
#include<iostream> 
using namespace std; 

class Apple 
{ 
    int i; 
    public: 
        Apple() 
        { 
            i = 0; 
            cout << "Inside Constructor\n"; 
        } 
        ~Apple() 
        { 
            cout << "Inside Destructor\n"; 
        } 
}; 

int main() 
{ 
    int x = 0; 
    if (x==0) 
    { 
        static Apple obj; 
    } 
    cout << "End of main\n"; 
}
```

 
输出：

``` c++
Inside Constructor
End of main
Inside Destructor
```

您可以清楚地看到输出的变化。现在，在main结束后调用析构函数。这是因为静态对象的范围是贯穿程序的生命周期。

### 类中的静态函数
就像类中的静态数据成员或静态变量一样，静态成员函数也不依赖于类的对象。我们被允许使用对象和'.'来调用静态成员函数。但建议使用类名和范围解析运算符调用静态成员。

允许静态成员函数仅访问静态数据成员或其他静态成员函数，它们无法访问类的非静态数据成员或成员函数。

``` c++
#include<iostream> 
using namespace std; 

class Apple 
{ 
    public: 
        // static member function 
        static void printMsg() 
        {
            cout<<"Welcome to Apple!"; 
        }
}; 

// main function 
int main() 
{ 
    // invoking a static member function 
    Apple::printMsg(); 
} 
```

# this指针那些时

首先来谈谈this指针的用处：

- （1）**一个对象的this指针并不是对象本身的一部分，不会影响sizeof(对象)的结果。**
- （2）this作用域是在类内部，当在类的非静态成员函数中访问类的非静态成员的时候，编译器会自动将对象本身的地址作为一个隐含参数传递给函数。也就是说，即使你没有写上this指针，编译器在编译的时候也是加上this的，**它作为非静态成员函数的隐含形参，对各成员的访问均通过this进行。**

其次，this指针的使用：

- （1）在类的非静态成员函数中返回类对象本身的时候，直接使用 return *this。
- （2）当参数与成员变量名相同时，如this->n = n （不能写成n = n)。

**总结：this在成员函数的开始执行前构造，在成员的执行结束后清除。上述的get_age函数会被解析成get_age(const A * const this),add_age函数会被解析成add_age(A* const this,int a)。在C++中类和结构是只有一个区别的：类的成员默认是private，而结构是public。this是类的指针，如果换成结构，那this就是结构的指针了。**

# inline 那些事
## 类中内联

``` c++
class A
{
public:
    void f1(int x); 

    /**
     * @brief 类中定义了的函数是隐式内联函数,声明要想成为内联函数，必须在实现处(定义处)加inline关键字。
     *
     * @param x
     * @param y
     */
    void Foo(int x,int y) ///< 定义即隐式内联函数！
    {

    };
    void f1(int x); ///< 声明后，要想成为内联函数，必须在定义处加inline关键字。  
};
```


``` c++
#include <iostream>
#include "inline.h"

using namespace std;

/**
 * @brief inline要起作用,inline要与函数定义放在一起,inline是一种“用于实现的关键字,而不是用于声明的关键字”
 *
 * @param x
 * @param y
 *
 * @return 
 */
int Foo(int x,int y);  // 函数声明
inline int Foo(int x,int y) // 函数定义
{
    return x+y;
}

// 定义处加inline关键字，推荐这种写法！
inline void A::f1(int x){

}

int main()
{
    cout<<Foo(1,2)<<endl;
}
/**
 * 编译器对 inline 函数的处理步骤
 * 将 inline 函数体复制到 inline 函数调用点处；
 * 为所用 inline 函数中的局部变量分配内存空间；
 * 将 inline 函数的的输入参数和返回值映射到调用方法的局部变量空间中；
 * 如果 inline 函数有多个返回点，将其转变为 inline 函数代码块末尾的分支（使用 GOTO）。
 */
```


> 编译器对 inline 函数的处理步骤

1. 将 inline 函数体复制到 inline 函数调用点处；
2. 为所有 inline 函数中的局部变量分配内存空间；
3. 将 inline 函数的的输入参数和返回值映射到调用方法的局部变量空间中；
4. 如果 inline 函数有多个返回点，将其转变为 inline 函数代码块末尾的分支（使用 GOTO）。

内联能提高函数效率，但并不是所有的函数都定义成内联函数！内联是以代码膨胀(复制)为代价，仅仅省去了函数调用的开销，从而提高函数的执行效率。

- 如果执行函数体内代码的时间相比于函数调用的开销较大，那么效率的收货会更少！
- 另一方面，每一处内联函数的调用都要复制代码，将使程序的总代码量增大，消耗更多的内存空间。

> 以下情况不宜用内联

- 如果函数体内的代码比较长，使得内联将导致内存消耗代价比较高。
- 如果函数体内出现循环，那么执行函数体内代码的时间要比函数调用的开销大。

## 虚函数（virtual）可以是内联函数（inline）吗？
- 虚函数可以是内联函数，内联是可以修饰虚函数的，但是当虚函数表现多态性的时候不能内联。
- 内联是在编译器建议编译器内联，而虚函数的多态性在运行期，编译器无法知道运行期调用哪个代码，因此虚函数表现为多态性时（运行期）不可以内联。
- inline virtual 唯一可以内联的时候是：编译器知道所调用的对象是哪个类（如 Base::who()），这只有在编译器具有实际对象而不是对象的指针或引用时才会发生。

``` c++
#include <iostream>
#include <istream>

using namespace std;

class Base
{
public:
  inline virtual void who() {
    cout << "I am Base\n";
  }
  virtual ~Base() {
    cout << "调用~Base()\n";
  }
};

class Derived : public Base {
public:
  inline void who() { // 不写inline时隐式内联
    cout << "I'm Derived\n";
  }
  ~Derived() {
    cout << "调用~Derived()\n";
  }
};

int main() {
  // 此处的虚函数 who() 是通过类(base) 的具体对象(b) 来调用的 编译期间就能确定了
  // 所以它可以是内联的，但最终是否内联取决于编译器。
  Base b; // 构造对象
  b.who();

  // 此处的虚函数事通过指针调用的，呈现多态性，需要运行期间才能确定，所以不能为内联
  Base *ptr = new Derived();
  ptr->who();

  // 因为Base有虚析构函数（virtual ~Base() {}），所以 delete
  // 时，会先调用派生类（Derived）析构函数，再调用基类（Base）析构函数，防止内存泄漏。
  delete ptr;
  ptr = nullptr;

  return 0;
}

```

# sizeof那些事
- 空类的大小为 1 byte
- 一个类中，虚函数本身、成员函数(包括静态与非静态)和静态数据成员都是不占用类对象的存储空间的
- 对于包含虚函数的类，不管有多少个虚函数，只有一个虚指针，vptr的大小
- 普通继承，派生类继承了所有积累的函数与成员，要按照字节对齐来计算大小
- 虚函数继承，不管是单继承还是多继承，都是继承了基类的vptr
- 虚继承 继承基类的vptr

``` c++
#include <iostream>

using namespace std;

class void_class {};

class non_void_class {
private:
  int da;
  void_class db;
  long dc;

public:
};

class Base {
public:
  virtual void vfunc() { cout << "this is Base.vfunc()\n"; }

  void who() { cout << "this is Base.who()\n"; }

private:
  static int a;
  static void_class b;
  static long c;
};

class VBase {
public:
  virtual void vfunc1() { cout << "this is VBase.vfunc1()\n"; }

  virtual void vfunc2() { cout << "this is VBase.vfunc2()\n"; }

  virtual void who() { cout << "this is Base.who()\n"; }

private:
  static int a;
  static void_class b;
  static long c;
};

class Derive : public Base {
private:
  int da;
  void_class db;
  long dc;

public:
};

class VDerive : public Base, public VBase {
private:
  int da;
  void_class db;
  long dc;

public:
  virtual void name() { cout << "I am VDerive\n"; }
};

int main() {
  void_class v;
  cout << "空类 " << sizeof(v) << endl;

  Base b;
  cout << "有虚函数 静态数据 " << sizeof(b) << endl; // 虚指针的大小

  non_void_class nv;
  cout << "非空类 有非静态数据 无虚函数 无继承 " << sizeof(nv) << endl;

  Derive d;
  cout << "单继承(父类有虚函数) 有非静态数据 " << sizeof(d) << endl;

  VDerive vd;
  cout << "多继承(父类有多个虚函数) 有非静态数据" << sizeof(vd) << endl;
}

```


# 纯虚函数和抽象类那些事

## 纯虚函数与抽象类
C++中的纯虚函数(或抽象函数)是我们没有实现的虚函数！我们只需声明它!通过声明中赋值0来声明纯虚函数！

``` c++
class Test 
{    
    // Data members of class 
public: 
    // Pure Virtual Function 
    virtual void show() = 0; 

    /* Other members */
}; 
```

- 纯虚函数：没有函数体的虚函数
- 抽象类：包含纯虚函数的类

``` c++
#include<iostream>

using namespace std;
class A
{
private:
    int a;
public:
    virtual void show()=0; // 纯虚函数
};


int main()
{
    /*
     * 1. 抽象类只能作为基类来派生新类使用
     * 2. 抽象类的指针和引用->由抽象类派生出来的类的对象！
     */
    A a; // error 抽象类，不能创建对象

    A *a1; // ok 可以定义抽象类的指针

    A *a2 = new A(); // error, A是抽象类，不能创建对象
}
```

抽象类只能作为基类来派生新类使用，不能创建抽象类的对象,抽象类的指针和引用->由抽象类派生出来的类的对象！
## 实现抽象类
**抽象类中：在成员函数内可以调用纯虚函数，在构造函数/析构函数内部不能使用纯虚函数。**

**如果一个类从抽象类派生而来，它必须实现了基类中的所有纯虚函数，才能成为非抽象类。**

``` c++
#include <iostream>

using namespace std;

class A {
public:
  virtual void f() = 0; // 纯虚函数
  void g() {
    this->f();
  }
};

class B:public A {
public:
  void f() {
    cout << "B:f()" << endl;
  }
};

int main() {
  B b;
  b.g();
  return 0;
}

```

## 重要点
- 纯虚函数使一个类变成抽象类

``` c++
#include <iostream>

using namespace std;

class Test {
  int x;

public:
  virtual void show() = 0;
  int getX() { return x; }
};

int main() {
  Test t; // error
  return 0;
}

```

- 抽象类类型的指针和引用

``` c++
#include <iostream>

using namespace std;

class Base {
  int x;

public:
  virtual void show() = 0;
  int getX() { return x; }
};

class Derived : public Base {
public:
  void show() { cout << "In Derived \n"; }
  Derived() {}
};

int main() {
  Base *bp = new Derived();
  bp->show();
  return 0;
}

```

- 如果我们不在派生类中覆盖纯虚函数，那么派生类也会变成抽象类。

``` c++
#include<iostream> 
using namespace std; 

class Base
{ 
    int x; 
public: 
    virtual void show() = 0; 
    int getX() { return x; } 
}; 
class Derived: public Base 
{ 
public: 
//    void show() { } 
}; 
int main(void) 
{ 
    Derived d;  //error! 派生类没有实现纯虚函数，那么派生类也会变为抽象类，不能创建抽象类的对象
    return 0; 
} 

```

- 抽象类可以有构造函数

``` c++
#include<iostream> 
using namespace std; 

// An abstract class with constructor 
class Base 
{ 
    protected: 
        int x; 
    public: 
        virtual void fun() = 0; 
        Base(int i) { x = i; } 
}; 

class Derived: public Base 
{ 
    int y; 
    public: 
    Derived(int i, int j):Base(i) { y = j; } 
    void fun() { cout << "x = " << x << ", y = " << y; } 
}; 

int main(void) 
{ 
    Derived d(4, 5); 
    d.fun(); 
    return 0; 
}
```

- 构造函数不能是虚函数，而析构函数可以是虚析构函数

``` c++

/**
 * @file interesting_facts5.cpp
 * @brief 构造函数不能是虚函数，而析构函数可以是虚析构函数。
 * 例如：当基类指针指向派生类对象并删除对象时，我们可能希望调用适当的析构函数。如果析构函数不是虚拟的，则只能调用基类析构函数。
 */
#include <iostream>
using namespace std;

class Base {
public:
  Base() { cout << "Constructor: Base" << endl; }
  virtual ~Base() { cout << "Destructor : Base" << endl; }
};

class Derived : public Base {
public:
  Derived() { cout << "Constructor: Derived" << endl; }
  ~Derived() { cout << "Destructor : Derived" << endl; }
};

int main() {
  Base *Var = new Derived();
  delete Var;
  return 0;
}


```

当基类指针指向派生类对象并删除对象时，我们可能希望调用适当的析构函数。如果析构函数不是虚拟的，则只能调用基类析构函数。

## 完整实例
抽象类由派生类继承实现！
``` c++
#include<iostream> 
using namespace std; 

class Base 
{ 
    int x; 
    public: 
    virtual void fun() = 0; 
    int getX() { return x; } 
}; 

class Derived: public Base 
{ 
    int y; 
    public: 
    void fun() { cout << "fun() called"; }  // 实现了fun()函数
}; 

int main(void) 
{ 
    Derived d; 
    d.fun(); 
    return 0; 
}
```

# vptr vtable那些事
## 基础理论
为了实现虚函数，C ++使用一种称为虚拟表的特殊形式的后期绑定。该虚拟表是用于解决在动态/后期绑定方式的函数调用函数的查找表。虚拟表有时会使用其他名称，例如“vtable”，“虚函数表”，“虚方法表”或“调度表”。

虚拟表实际上非常简单，虽然用文字描述有点复杂。首先，每个使用虚函数的类（或者从使用虚函数的类派生）都有自己的虚拟表。该表只是编译器在编译时设置的静态数组。虚拟表包含可由类的对象调用的每个虚函数的一个条目。此表中的每个条目只是一个函数指针，指向该类可访问的派生函数。

其次，编译器还会添加一个隐藏指向基类的指针，我们称之为vptr。vptr在创建类实例时自动设置，以便指向该类的虚拟表。与this指针不同，this指针实际上是编译器用来解析自引用的函数参数，vptr是一个真正的指针。

因此，它使每个类对象的分配大一个指针的大小。这也意味着vptr由派生类继承，这很重要。

## 实现与内部结构
下面我们来看自动与手动操纵vptr来获取地址与调用虚函数！

开始看代码之前，为了方便大家理解，这里给出调用图：

![img](https://light-city.club/sc/assets/images/base.jpg)

``` c++
#include <cstdint>
#include <iostream>
#include <stdio.h>

using namespace std;

// 函数指针
typedef void (*Fun)();

class Base {
public:
  Base() {}
  virtual void fun1() { cout << "Base::fun1()" << endl; }
  virtual void fun2() { cout << "Base::fun2()" << endl; }
  virtual void fun3() { cout << "Base::fun3()" << endl; }
  ~Base() {}
};

class Derived : public Base {
public:
  Derived(){};
  void fun1() { cout << "Derived::fun1()" << endl; }
  void fun2() { cout << "Derived::fun2()" << endl; }
  ~Derived(){};
};

Fun getAddr(void *obj, unsigned int offset) {
  cout << "========================" << endl;
  // 64位操作系统，占8字节，通过*(uint64_t *)obj取出前8字节，即vptr指针
  void *vptr_addr = (void *)*(uint64_t *)obj;
  printf("vptr_addr:%p\n", vptr_addr);

  void *func_addr = (void *)*((uint64_t *)vptr_addr + offset);
  printf("func_addr:%p\n", func_addr);
  return (Fun)func_addr;
}

int main() {
  Base ptr;
  Derived d;

  Base *pt = new Derived(); // 基类指针指向派生类实例
  Base &pp = ptr;           // 基类引用指向基类实例
  Base &p = d;              // 基类引用指向派生类实例

  cout << "基类对象直接调用" << endl;
  ptr.fun1();
  cout << "基类对象调用基类实例" << endl;
  pp.fun1();
  cout << "基类指针指向派生类实例并调用虚函数" << endl;
  pt->fun1();
  cout << "基类引用指向派生类实例并调用虚函数" << endl;
  p.fun1();

  // 手动查找vptr 和 vtable
  Fun f1 = getAddr(pt, 0);
  (*f1)();

  Fun f2 = getAddr(pt, 1);
  (*f2)();

  Fun f3 = getAddr(pt, 2);
  (*f3)();

  Fun f4 = getAddr(&ptr, 2);
  (*f4)();

  cout << "基类对象的大小" << endl;
  cout << sizeof(ptr) << endl;
  cout << "派生类对象的大小" << endl;
  cout << sizeof(*pt) << endl;

  delete pt;
  return 0;
}

```

``` c++
基类对象直接调用
Base::fun1()
基类对象调用基类实例
Base::fun1()
基类指针指向派生类实例并调用虚函数
Derived::fun1()
基类引用指向派生类实例并调用虚函数
Derived::fun1()
========================
vptr_addr:0x55f6b0b27d50
func_addr:0x55f6b0b256a6
Derived::fun1()
========================
vptr_addr:0x55f6b0b27d50
func_addr:0x55f6b0b256e0
Derived::fun2()
========================
vptr_addr:0x55f6b0b27d50
func_addr:0x55f6b0b25628
Base::fun3()
========================
vptr_addr:0x55f6b0b27d78
func_addr:0x55f6b0b25628
Base::fun3()
基类对象的大小
8
派生类对象的大小
8

```

我们发现C++的动态多态性是通过虚函数来实现的。简单的说，通过virtual函数，指向子类的基类指针可以调用子类的函数。例如，上述通过基类指针指向派生类实例，并调用虚函数，将上述代码简化为：

``` c++
Base *pt = new Derived(); // 基类指针指向派生类实例
cout<<"基类指针指向派生类实例并调用虚函数"<<endl;
pt->fun1();
```

其过程为：首先程序识别出fun1()是个虚函数，其次程序使用pt->vptr来获取Derived的虚拟表。第三，它查找Derived虚拟表中调用哪个版本的fun1()。这里就可以发现调用的是Derived::fun1()。因此pt->fun1()被解析为Derived::fun1()!



# virtual那些事

## 虚函数与运行多态

虚函数的调用取决于指向或者引用的对象的类型，而不是指针或者引用自身的类型。

``` c++
#include <iostream>

using namespace std;

class Employee {
public:
  virtual void raiseSalary() { cout << 0 << endl; }
  virtual void promote() {}
};

class Manager : public Employee {
  virtual void raiseSalary() { cout << 100 << endl; }
  virtual void promote() {}
};

class Engineer : public Employee {
  virtual void raiseSalary() { cout << 200 << endl; }
  virtual void promote() {}
};

void globalRaiseSalary(Employee *emp[], int n) {
  for (int i = 0;i < n;i++) {
    emp[i]->raiseSalary();
  }
}

int main() {
  Employee *emp[] = {
    new Manager(),
    new Engineer
  };
  globalRaiseSalary(emp, sizeof(emp)/sizeof(*emp));
  return 0;

}

```

## vptr与vtable

见上章

## 虚函数中默认参数

**默认参数是静态绑定的，虚函数是动态绑定的。 默认参数的使用需要看指针或者引用本身的类型，而不是对象的类型。或者说默认参数的字典中没有virtual**

``` c++
#include <iostream>

using namespace std;

class Base {
public:
  virtual void fun(int x = 10) { cout << "Base::fun(), x = " << x << endl; }
};

class Derived : public Base {
public:
  virtual void fun(int x = 20) { cout << "Derived::fun(), x = " << x << endl; }
};

int main() {
  Derived d1;
  Base *bp = &d1;
  bp->fun();
  return 0;
}

```

## 可以不可以

> 静态函数可以声明为虚函数吗？

原因主要有两方面：

**静态函数不可以声明为虚函数，同时也不能被const 和 volatile 关键字修饰**

static成员函数不属于任何类对象或类实例，所以即使给此函数加上virutal也是没有任何意义

虚函数依靠vptr和vtable来处理。vptr是一个指针，在类的构造函数中创建生成，并且只能用this指针来访问它，静态成员函数没有this指针，所以无法访问vptr。

``` c++
virtual static void fun()  { }
static void fun() const { }
```

> 构造函数可以为虚函数吗？ 

**构造函数不可以声明为虚函数。同时除了inline|explicit之外，构造函数不允许使用其它任何关键字。**

为什么构造函数不可以为虚函数？

尽管虚函数表vtable是在编译阶段就已经建立的，但指向虚函数表的指针vptr是在运行阶段实例化对象时才产生的。 如果类含有虚函数，编译器会在构造函数中添加代码来创建vptr。 问题来了，如果构造函数是虚的，那么它需要vptr来访问vtable，可这个时候vptr还没产生。 因此，构造函数不可以为虚函数。

我们之所以使用虚函数，是因为需要在信息不全的情况下进行多态运行。而构造函数是用来初始化实例的，实例的类型必须是明确的。 因此，构造函数没有必要被声明为虚函数。


``` c++
/**
 * @file vir_con.cpp
 * @brief 构造函数不可以声明为虚函数。同时除了inline之外，构造函数不允许使用其它任何关键字。
 *
 * 为什么构造函数不可以为虚函数？
 *
 * 尽管虚函数表vtable是在编译阶段就已经建立的，但指向虚函数表的指针vptr是在运行阶段实例化对象时才产生的。 如果类含有虚函数，编译器会在构造函数中添加代码来创建vptr。 问题来了，如果构造函数是虚的，那么它需要vptr来访问vtable，可这个时候vptr还没产生。 因此，构造函数不可以为虚函数。
 * 我们之所以使用虚函数，是因为需要在信息不全的情况下进行多态运行。而构造函数是用来初始化实例的，实例的类型必须是明确的。
 * 因此，构造函数没有必要被声明为虚函数。
 * 尽管构造函数不可以为虚函数，但是有些场景下我们确实需要 “Virtual Copy Constructor”。 “虚复制构造函数”的说法并不严谨，其只是一个实现了对象复制的功能的类内函数。 举一个应用场景，比如剪切板功能。 复制内容作为基类，但派生类可能包含文字、图片、视频等等。 我们只有在程序运行的时候才知道我们需要复制的具体是什么类型的数据。
 *
 */
 
#include <iostream>

using namespace std;

class Base {
public:
  Base() {}

  virtual ~Base() {}

  virtual void ChangeAttributes() = 0;

  // "Virtual Constructor"
  static Base *Create(int id);

  // The "Virtual Copy Constructor"
  virtual Base *Clone() = 0;
};

class Derived1 : public Base {
public:
  Derived1() { cout << "Derived1 created" << endl; }

  Derived1(const Derived1 &rhs) {
    cout << "Derived1 created by deep copy" << endl;
  }

  ~Derived1() { cout << "~Derived1 destroyed" << endl; }

  void ChangeAttributes() { cout << "Derived1 Attributes Changed" << endl; }

  Base *Clone() { return new Derived1(*this); }
};

class Derived2 : public Base {

public:
  Derived2() { cout << "Derived2 created" << endl; }

  Derived2(const Derived2 &rhs) {
    cout << "Derived2 created by deep copy" << endl;
  }

  ~Derived2() { cout << "~Derived2 destroyed" << endl; }

  void ChangeAttributes() { cout << "Derived2 Attributes Changed" << endl; }

  Base *Clone() { return new Derived2(*this); }
};

class Derived3 : public Base {
public:
  Derived3() { cout << "Derived3 created" << endl; }

  Derived3(const Derived3 &rhs) {
    cout << "Derived3 created by deep copy" << endl;
  }

  ~Derived3() { cout << "~Derived3 destroyed" << endl; }

  void ChangeAttributes() { cout << "Derived3 Attributes Changed" << endl; }

  Base *Clone() { return new Derived3(*this); }
};

Base *Base::Create(int id) {
  if (id == 1) {
    return new Derived1;
  } else if (id == 2) {
    return new Derived2;
  } else {
    return new Derived3;
  }
}

class User {
public:
  User() : pBase(nullptr) {
    int input;
    cout << "Enter ID (1, 2 or 3): ";
    cin >> input;

    while ((input != 1) && (input != 2) && (input != 3)) {
      cout << "Enter ID (1, 2 or 3 only): ";
      cin >> input;
    }

    // Create objects via the "Virtual Constructor"
    pBase = Base::Create(input);
  }

  ~User() {
    if (pBase) {
      delete pBase;
      pBase = nullptr;
    }
  }

  void Action() {
    Base *pNewBase = pBase->Clone();
    pNewBase->ChangeAttributes();
    delete pNewBase;
  }

private:
  Base *pBase;
};

int main() {
  User *user = new User();

  user->Action();

  delete user;
}

```

> 析构函数可以为虚函数吗？

**析构函数可以声明为虚函数。如果我们需要删除一个指向派生类的基类指针时，应该把析构函数声明为虚函数。 事实上，只要一个类有可能会被其它类所继承， 就应该声明虚析构函数(哪怕该析构函数不执行任何操作)。**

``` c++
#include<iostream> 

using namespace std; 

class base { 
    public: 
        base()      
        { cout<<"Constructing base \n"; } 
        virtual ~base() 
        { cout<<"Destructing base \n"; }      
}; 

class derived: public base { 
    public: 
        derived()      
        { cout<<"Constructing derived \n"; } 
        ~derived() 
        { cout<<"Destructing derived \n"; } 
}; 

int main(void) 
{ 
    derived *d = new derived();   
    base *b = d; 
    delete b; 
    return 0; 
}
```

> 虚函数可以为私有函数吗？

- 基类指针指向继承类对象，则调用继承类对象的函数；
- int main()必须声明为Base类的友元，否则编译失败。 编译器报错： ptr无法访问私有函数。 当然，把基类声明为public， 继承类为private，该问题就不存在了。

``` c++
#include<iostream> 
using namespace std; 

// 以下两种都是可以的
class Derived; 

class Base { 
    private: 
        virtual void fun() { cout << "Base Fun"; } 
        friend int main(); 
}; 

class Derived: public Base { 
    public: 
        void fun() { cout << "Derived Fun"; } 
}; 

class Derived; 

class Base { 
    public: 
        virtual void fun() { cout << "Base Fun"; } 
     //   friend int main(); 
}; 

class Derived: public Base { 
    private: 
        void fun() { cout << "Derived Fun"; } 
};

int main() 
{ 
    Base *ptr = new Derived; 
    ptr->fun(); 
    return 0; 
}

```

> 虚函数可以被内联吗？

通常类成员函数都会被编译器考虑是否进行内联。 但通过基类指针或者引用调用的虚函数必定不能被内联。 当然，实体对象调用虚函数或者静态调用时可以被内联，虚析构函数的静态调用也一定会被内联展开。

- 虚函数可以是内联函数，内联是可以修饰虚函数的，但是当虚函数表现多态性的时候不能内联。
- 内联是在编译器建议编译器内联，而虚函数的多态性在运行期，编译器无法知道运行期调用哪个代码，因此虚函数表现为多态性时（运行期）不可以内联。
- inline virtual 唯一可以内联的时候是：编译器知道所调用的对象是哪个类（如 Base::who()），这只有在编译器具有实际对象而不是对象的指针或引用时才会发生。

``` c++
#include <iostream> 
using namespace std; 
class Base 
{ 
    public: 
        virtual void who() 
        { 
            cout << "I am Base\n"; 
        } 
}; 
class Derived: public Base 
{ 
    public: 
        void who() 
        {  
            cout << "I am Derived\n"; 
        } 
}; 

int main() 
{ 
    // note here virtual function who() is called through 
    // object of the class (it will be resolved at compile 
    // time) so it can be inlined. 
    Base b; 
    b.who(); 

    // Here virtual function is called through pointer, 
    // so it cannot be inlined 
    Base *ptr = new Derived(); 
    ptr->who(); 

    return 0; 
} 
```

## RTTI与dynamic_cast

RTTI（Run-Time Type Identification)，通过运行时类型信息程序能够使用基类的指针或引用来检查这些指针或引用所指的对象的实际派生类型。

在面向对象程序设计中，有时我们需要在运行时查询一个对象是否能作为某种多态类型使用。与Java的instanceof，以及C#的as、is运算符类似，C++提供了dynamic_cast函数用于动态转型。相比C风格的强制类型转换和C++ reinterpret_cast，dynamic_cast提供了类型安全检查，是一种基于能力查询(Capability Query)的转换，所以在多态类型间进行转换更提倡采用dynamic_cast。

``` c++
#include <iostream>
#include <typeinfo>

using namespace std;

class B {
  virtual void func() {}
};

class D : public B {};

int main() {
  B *b = new D; // 向上转型
  B &obj = *b;
  D *d = dynamic_cast<D *>(b); // 向下转型
  if (d != nullptr) {
    cout << "cast B* to D*\n";
  } else {
    cout << "cannot cast B* to D*\n";
  }

  try {
    D &dobj = dynamic_cast<D &>(obj);
    cout << "works!\n";
  } catch (bad_cast bc) {
    cout << bc.what() << endl;
  }
  return 0;
}

```

-  在使用时需要注意：被转换对象obj的类型T1必须是多态类型，即T1必须公有继承自其它类，或者T1拥有虚函数（继承或自定义）。若T1为非多态类型，使用dynamic_cast会报编译错误。
``` c++
// 在使用时需要注意：被转换对象obj的类型T1必须是多态类型，即T1必须公有继承自其它类，或者T1拥有虚函数（继承或自定义）。若T1为非多态类型，使用dynamic_cast会报编译错误。

// A为非多态类型 

class A{

};

//B为多态类型

class B{ 

    public: virtual ~B(){}

};

//D为多态类型

class D: public A{

};

//E为非多态类型

class E : private A{

};

//F为多态类型

class F : private B{

}


```

## 纯虚函数和抽象类

见上章


# volatile那些事
## volatile
被 volatile 修饰的变量，在对其进行读写操作时，会引发一些可观测的副作用。而这些可观测的副作用，是由程序之外的因素决定的。

说人话就是告诉编译器这个变量的内容不管如何修饰，它就是会变的, 不必去管它(优化)。

## volatile应用

> （1）并行设备的硬件寄存器（如状态寄存器）。 假设要对一个设备进行初始化，此设备的某一个寄存器为0xff800000。

``` c++
int  *output = (unsigned  int *)0xff800000; //定义一个IO端口；  
int   init(void)  
{  
    int i;  
    for(i=0;i< 10;i++)
    {  
    *output = i;  
    }  
}
```

经过编译器优化后，编译器认为前面循环半天都是废话，对最后的结果毫无影响，因为最终只是将output这个指针赋值为 9，所以编译器最后给你编译编译的代码结果相当于：

``` c++
int  init(void)  
{  
    *output = 9;  
}
```

如果你对此外部设备进行初始化的过程是必须是像上面代码一样顺序的对其赋值，显然优化过程并不能达到目的。反之如果你不是对此端口反复写操作，而是反复读操作，其结果是一样的，编译器在优化后，也许你的代码对此地址的读操作只做了一次。然而从代码角度看是没有任何问题的。这时候就该使用volatile通知编译器这个变量是一个不稳定的，在遇到此变量时候不要优化。

> （2）一个中断服务子程序中访问到的变量；

``` c++
static int i=0;

int main()
{
    while(1)
    {
    if(i) dosomething();
    }
}

/* Interrupt service routine */
void IRS()
{
    i=1;
}
```

上面示例程序的本意是产生中断时，由中断服务子程序IRS响应中断，变更程序变量i，使在main函数中调用dosomething函数，但是，由于编译器判断在main函数里面没有修改过i，因此可能只执行一次对从i到某寄存器的读操作，然后每次if判断都只使用这个寄存器里面的“i副本”，导致dosomething永远不会被调用。如果将变量i加上volatile修饰，则编译器保证对变量i的读写操作都不会被优化，从而保证了变量i被外部程序更改后能及时在原程序中得到感知。

> （3）多线程应用中被多个任务共享的变量;

当多个线程共享某一个变量时，该变量的值会被某一个线程更改，应该用 volatile 声明。作用是防止编译器优化把变量从内存装入CPU寄存器中，当一个线程更改变量后，未及时同步到其它线程中导致程序出错。volatile的意思是让编译器每次操作该变量时一定要从内存中真正取出，而不是使用已经存在寄存器中的值。

``` c++
volatile  bool bStop=false;  //bStop 为共享全局变量  
//第一个线程
void threadFunc1()
{
    ...
    while(!bStop){...}
}
//第二个线程终止上面的线程循环
void threadFunc2()
{
    ...
    bStop = true;
}
```

要想通过第二个线程终止第一个线程循环，如果bStop不使用volatile定义，那么这个循环将是一个死循环，因为bStop已经读取到了寄存器中，寄存器中bStop的值永远不会变成FALSE，加上volatile，程序在执行时，每次均从内存中读出bStop的值，就不会死循环了。

是否了解volatile的应用场景是区分C/C++程序员和嵌入式开发程序员的有效办法，搞嵌入式的家伙们经常同硬件、中断、RTOS等等打交道，这些都要求用到volatile变量，不懂得volatile将会带来程序设计的灾难。


## volatile常见问题
下面的问题可以看一下面试者是不是直正了解volatile。
> （1）一个参数既可以是const还可以是volatile吗？为什么？ 

可以。一个例子是只读的状态寄存器。它是volatile因为它可能被意想不到地改变。它是const因为程序不应该试图去修改它。

> （2）一个指针可以是volatile吗？为什么？ 

可以。尽管这并不常见。一个例子是当一个中断服务子程序修该一个指向一个buffer的指针时。

> （3）下面的函数有什么错误？

``` c++
int square(volatile int *ptr) 
{ 
return *ptr * *ptr; 
}
```
 
这段代码有点变态，其目的是用来返回指针ptr指向值的平方，但是，由于ptr指向一个volatile型参数，编译器将产生类似下面的代码：

``` c++
int square(volatile int *ptr) 
{ 
  int a,b; 
  a = *ptr; 
  b = *ptr; 
  return a * b; 
}
```

 
由于*ptr的值可能被意想不到地改变，因此a和b可能是不同的。结果，这段代码可能返回的不是你所期望的平方值！正确的代码如下：

``` c++
long square(volatile int *ptr) 
{ 
  int a=*ptr; 
  return a * a; 
}
```


## volatile使用

- volatile 关键字是一种类型修饰符，用它声明的类型变量表示可以被某些编译器未知的因素（操作系统、硬件、其它线程等）更改。所以使用 volatile 告诉编译器不应对这样的对象进行优化。
- volatile 关键字声明的变量，每次访问时都必须从内存中取出值（没有被 volatile 修饰的变量，可能由于编译器的优化，从 CPU 寄存器中取值）
- const 可以是 volatile （如只读的状态寄存器）
- 指针可以是 volatile

``` c++
/* Compile code without optimization option */
#include <stdio.h>
int main(void)
{
  //const int local = 10;
  const volatile int local = 10;
  int *ptr = (int*) &local;

  printf("Initial value of local : %d \n", local);

  *ptr = 100;

  printf("Modified value of local: %d \n", local);

  return 0;
}

```

# assert那些事

> 断言，是宏，而非函数。assert 宏的原型定义在 （C）、（C++）中，其作用是如果它的条件返回错误，则终止程序执行。可以通过定义 NDEBUG 来关闭 assert，但是需要在源代码的开头，include 之前。

``` c++
void assert(int expression);

#include <stdio.h> 
#include <assert.h> 

int main() 
{ 
    int x = 7; 

    /*  Some big code in between and let's say x  
    is accidentally changed to 9  */
    x = 9; 

    // Programmer assumes x to be 7 in rest of the code 
    assert(x==7); 

    /* Rest of the code */

    return 0; 
} 
```

> 断言与正常错误处理

断言主要用于检查逻辑上不可能的情况。例如，它们可用于检查代码在开始运行之前所期望的状态，或者在运行完成后检查状态。与正常的错误处理不同，断言通常在运行时被禁用。

> 忽略断言：

在代码开头加上：

``` c++
#define NDEBUG          // 加上这行，则 assert 不可用
```

# 位域那些事

“ 位域 “ 或 “ 位段 “(Bit field)为一种数据结构，可以把数据以位的形式紧凑的储存，并允许程序员对此结构的位进行操作。这种数据结构的一个好处是它可以使数据单元节省储存空间，当程序需要成千上万个数据单元时，这种方法就显得尤为重要。第二个好处是位段可以很方便的访问一个整数值的部分内容从而可以简化程序源代码。而这种数据结构的缺点在于，位段实现依赖于具体的机器和系统，在不同的平台可能有不同的结果，这导致了位段在本质上是不可移植的。

- 位域在内存中的布局是与机器有关的
- 位域的类型必须是整型或枚举类型，带符号类型中的位域的行为将因具体实现而定
- 取地址运算符（&）不能作用于位域，任何指针都无法指向类的位域


# extern那些事
## C++与C编译区别
在C++中常在头文件见到extern "C"修饰函数，那有什么作用呢？ 是用于C++链接在C语言模块中定义的函数。

C++虽然兼容C，但C++文件中函数编译后生成的符号与C语言生成的不同。因为C++支持函数重载，C++函数编译后生成的符号带有函数参数类型的信息，而C则没有。

例如int add(int a, int b)函数经过C++编译器生成.o文件后，add会变成形如add_int_int之类的, 而C的话则会是形如_add, 就是说：相同的函数，在C和C++中，编译后生成的符号不同。

这就导致一个问题：如果C++中使用C语言实现的函数，在编译链接的时候，会出错，提示找不到对应的符号。此时extern "C"就起作用了：告诉链接器去寻找_add这类的C语言符号，而不是经过C++修饰的符号。

## C++调用C函数

C++调用C函数的例子: 引用C的头文件时，需要加extern "C"

``` c++
//add.h
#ifndef ADD_H
#define ADD_H
int add(int x,int y);
#endif

//add.c
#include "add.h"

int add(int x,int y) {
    return x+y;
}

//add.cpp
#include <iostream>
#include "add.h"
using namespace std;
int main() {
    add(2,3);
    return 0;
}
```


编译：

``` c++
//Generate add.o file

gcc -c add.c
```

链接：

``` c++
g++ add.cpp add.o -o main
```

没有添加extern "C" 报错：

``` c++
g++ add.cpp add.o -o main                                   
add.o：在函数‘main’中：
add.cpp:(.text+0x0): `main'被多次定义
/tmp/ccH65yQF.o:add.cpp:(.text+0x0)：第一次在此定义
/tmp/ccH65yQF.o：在函数‘main’中：
add.cpp:(.text+0xf)：对‘add(int, int)’未定义的引用
add.o：在函数‘main’中：
add.cpp:(.text+0xf)：对‘add(int, int)’未定义的引用
collect2: error: ld returned 1 exit status

```

添加extern "C"后：

add.cpp
``` c++
#include <iostream>
using namespace std;
extern "C" {
    #include "add.h"
}
int main() {
    add(2,3);
    return 0;
}
```

编译的时候一定要注意，先通过gcc生成中间文件add.o。

``` c++

gcc -c add.c 
```

然后编译：

``` c++
g++ add.cpp add.o -o main
```


## C中调用C++函数

综上，总结出使用方法，在C语言的头文件中，对其外部函数只能指定为extern类型，C语言中不支持extern "C"声明，在.c文件中包含了extern "C"时会出现编译语法错误。所以使用extern "C"全部都放在于cpp程序相关文件或其头文件中。

extern "C"在C中是语法错误，需要放在C++头文件中。

``` c++
// add.h
#ifndef ADD_H
#define ADD_H
extern "C" {
    int add(int x,int y);
}
#endif

// add.cpp
#include "add.h"

int add(int x,int y) {
    return x+y;
}

// add.c
extern int add(int x,int y);
int main() {
    add(2,3);
    return 0;
}
```

编译：

``` c++
g++ -c add.cpp
```

链接：

``` c++

gcc add.c add.o -o main
```

# struct那些事
|C|C++|
|:-:|:-:|
|不能将函数放在结构体声明|能将函数放在结构体声明|
|在C结构体声明中不能使用C++访问修饰符。|public、protected、private 在C++中可以使用。|
|在C中定义结构体变量，如果使用了下面定义必须加struct。|可以不加struct|
|结构体不能继承（没有这一概念）。|可以继承|
|若结构体的名字与函数名相同，可以正常运行且正常的调用！|若结构体的名字与函数名相同，使用结构体，只能使用带struct定义！|

# struct与class那些事

总的来说，struct 更适合看成是一个数据结构的实现体，class 更适合看成是一个对象的实现体。

区别:

- 最本质的一个区别就是默认的访问控制

- 默认的继承访问权限。struct 是 public 的，class 是 private 的。

- struct 作为数据结构的实现体，它默认的数据访问控制是 public 的，而 class 作为对象的实现体，它默认的成员变量访问控制是 private 的。

``` c++
#include <iostream>
#include <cstdio>

using namespace std;

struct Base {
private:
  int data1;
public:
  Base(int d) {
    data1 = d;
    cout << "Base()" << endl;
  }
};

class Derived : public Base {

};

int main() {
  struct Base b(1);
}

```

# union那些事
联合（union）是一种节省空间的特殊的类，一个 union 可以有多个数据成员，但是在任意时刻只有一个数据成员可以有值。当某个成员被赋值后其他成员变为未定义状态。联合有如下特点：

- 默认访问控制符为 public
- 可以含有构造函数、析构函数
- **不能含有引用类型的成员**
- **不能继承自其他类，不能作为基类**
- **不能含有虚函数**
- 匿名 union 在定义所在作用域可直接访问 union 成员
- 匿名 union 不能包含 protected 成员或 private 成员
- 全局匿名联合必须是静态（static）的

# c实现c++那多态那些事
## C++实现案例
> C++中的多态

在C++中会维护一张虚函数表，根据赋值兼容规则，我们知道父类的指针或者引用是可以指向子类对象的。

如果一个父类的指针或者引用调用父类的虚函数则该父类的指针会在自己的虚函数表中查找自己的函数地址，如果该父类对象的指针或者引用指向的是子类的对象，而且该子类已经重写了父类的虚函数，则该指针会调用子类的已经重写的虚函数。

``` c++
#include <iostream>

using namespace std;
class A {
public:
  virtual void f() //虚函数实现
  {
    cout << "Base A::f() " << endl;
  }
};

class B : public A // 必须为共有继承，否则后面调用不到，class默认为私有继承！
{
public:
  virtual void f() //虚函数实现,子类中virtual关键字可以没有
  {
    cout << "Derived B::f() " << endl;
  }
};

int main() {
  A a; //基类对象
  B b; //派生类对象

  A *pa = &a; //父类指针指向父类对象
  pa->f();    //调用父类的函数

  pa = &b; //父类指针指向子类对象，多态实现
  pa->f(); //调用派生类同名函数
  return 0;
}

```

## C实现
## 封装
C语言中是没有class类这个概念的，但是有struct结构体，我们可以考虑使用struct来模拟；

> 使用函数指针把属性与方法封装到结构体中。

## 继承

> 结构体嵌套

## 多态

> 类与子类方法的函数指针不同

在C语言的结构体内部是没有成员函数的，如果实现这个父结构体和子结构体共有的函数呢？我们可以考虑使用函数指针来模拟。但是这样处理存在一个缺陷就是：父子各自的函数指针之间指向的不是类似C++中维护的虚函数表而是一块物理内存，如果模拟的函数过多的话就会不容易维护了。

模拟多态，必须保持函数指针变量对齐(在内容上完全一致，而且变量对齐上也完全一致)。否则父类指针指向子类对象，运行崩溃！

``` c++
#include <stdio.h>

typedef void (*pf)();

typedef struct _A {
  pf _f;
}A;

typedef struct _B {
  A _b; // 继承
}B;


void FunA() {
  printf("%s\n", "Base A::fun()");
}

void FunB() {
  printf("%s\n", "Derived B::fun()");
}

int main() {
  A a;
  B b;

  a._f = FunA;
  b._b._f = FunB;

  A *pa = &a;
  pa->_f();
  pa = (A*)&b;
  pa->_f();
  return 0;
}

```

# explicit那些事

- explicit 修饰构造函数时，可以防止隐式转换和复制初始化
- explicit 修饰转换函数时，可以防止隐式转换，但按语境转换除外

# friend那些事

友元提供了一种 普通函数或者类成员函数 访问另一个类中的私有或保护成员 的机制。也就是说有两种形式的友元：

1. 友元函数：普通函数对一个访问某个类中的私有或保护成员。
2. 友元类：类A中的成员函数访问类B中的私有或保护成员

- 优点：提高了程序的运行效率。
- 缺点：破坏了类的封装性和数据的透明性。

总结： - 能访问私有成员 - 破坏封装性 - 友元关系不可传递 - 友元关系的单向性 - 友元声明的形式及数量不受限制

## 友元函数

注意，友元函数只是一个普通函数，并不是该类的类成员函数，它可以在任何地方调用，友元函数中通过对象名来访问该类的私有或保护成员。

``` c++
#include <iostream>

using namespace std;

class A
{
public:
    A(int _a):a(_a){};
    friend int geta(A &ca);  ///< 友元函数
private:
    int a;
};

int geta(A &ca) 
{
    return ca.a;
}

int main()
{
    A a(3);    
    cout<<geta(a)<<endl;

    return 0;
}
```


## 友元类

``` c++
#include <iostream>

using namespace std;

class A
{
public:
    A(int _a):a(_a){};
    friend class B;
private:
    int a;
};

class B
{
public:
    int getb(A ca) {
        return  ca.a; 
    };
};

int main() 
{
    A a(3);
    B b;
    cout<<b.getb(a)<<endl;
    return 0;
  }
```


## 注意
- 友元关系没有继承性 假如类B是类A的友元，类C继承于类A，那么友元类B是没办法直接访问类C的私有或保护成员。
- 友元关系没有传递性 假如类B是类A的友元，类C是类B的友元，那么友元类C是没办法直接访问类A的私有或保护成员，也就是不存在“友元的友元”这种关系。

# using那些事
## 基本使用

``` c++
#include <iostream>
#define isNs1 1
//#define isGlobal 2
using namespace std;
void func() 
{
    cout<<"::func"<<endl;
}

namespace ns1 {
    void func()
    {
        cout<<"ns1::func"<<endl; 
    }
}

namespace ns2 {
#ifdef isNs1 
    using ns1::func;    /// ns1中的函数
#elif isGlobal
    using ::func; /// 全局中的函数
#else
    void func() 
    {
        cout<<"other::func"<<endl; 
    }
#endif
}

int main() 
{
    /**
     * 这就是为什么在c++中使用了cmath而不是math.h头文件
     */
    ns2::func(); // 会根据当前环境定义宏的不同来调用不同命名空间下的func()函数
    return 0;
}
```

## 改变访问性

``` c++
#include <iostream>

using namespace std;

class Base1 {
public:
  Base1() : value(10) {}
  virtual ~Base1() {}
  void test1() { cout << "Base test1..." << endl; }

protected: // 保护
  int value;
};
// 默认为私有继承
class Derived1 : Base1 {
public:
  void test2() { cout << "value is " << value << endl; }
};

class Base {
public:
  Base() : value(20) {}
  virtual ~Base() {}
  void test1() { cout << "Base test1..." << endl; }

private: //私有
  int value;
};

/**
 * 子类对父类成员的访问权限跟如何继承没有任何关系，
 * “子类可以访问父类的public和protected成员，不可以访问父类的private成员”——这句话对任何一种继承都是成立的。
 *
 */
class Derived : Base {
public:
  using Base::value;
  void test2() { cout << "value is " << value << endl; }
};

int main() {
  Derived1 d1;
  d1.test2();

  Derived d;
  d.test2();
  return 0;
}

```


``` c++

class Base{
public:
 std::size_t size() const { return n;  }
protected:
 std::size_t n;
};
class Derived : private Base {
public:
 using Base::size;
protected:
 using Base::n;
};
```

类Derived私有继承了Base，对于它来说成员变量n和成员函数size都是私有的，如果使用了using语句，可以改变他们的可访问性，如上述例子中，size可以按public的权限访问，n可以按protected的权限访问。

## 函数重载

**在继承过程中，派生类可以覆盖重载函数的0个或多个实例，一旦定义了一个重载版本，那么其他的重载版本都会变为不可见。**

如果对于基类的重载函数，我们需要在派生类中修改一个，又要让其他的保持可见，必须要重载所有版本，这样十分的繁琐。

``` c++
#include <iostream>

using namespace std;


class Base {
public:
  Base(int i = 0) : data(i) {

  }
  void f() {
    cout << data << endl;
  }

  void f(int i) {
    cout << data << endl;
  }

  void f(int i) const {
    cout << data << endl;
  }
protected:
  int data;
};

class Derived : public Base {
public:
  using Base::f;
  void f(int i) {
    data = i;
    cout << "Derived=" << data << endl;
  }

};

int main() {
  Base b(20);
  b.f();

  Derived d;
  d.f(10);
  d.f();
}

```

**如上代码中，在派生类中使用using声明语句指定一个名字而不指定形参列表，所以一条基类成员函数的using声明语句就可以把该函数的所有重载实例添加到派生类的作用域中。此时，派生类只需要定义其特有的函数就行了，而无需为继承而来的其他函数重新定义。**

## 取代typedef
C中常用typedef A B这样的语法，将B定义为A类型，也就是给A类型一个别名B

对应typedef A B，使用using B=A可以进行同样的操作。

``` c++
typedef vector<int> V1; 
using V2 = vector<int>;
```

# ::那些事 
- 全局作用域符（::name）：用于类型名称（类、类成员、成员函数、变量等）前，表示作用域为全局命名空间
- 类作用域符（class::name）：用于表示指定类型的作用域范围是具体某个类的
- 命名空间作用域符（namespace::name）:用于表示指定类型的作用域范围是具体某个命名空间的

# enum那些事 
## 传统行为
- 作用域不受限,会容易引起命名冲突。例如下面无法编译通过的：

``` c++
#include <iostream>
using namespace std;

enum Color {RED,BLUE};
enum Feeling {EXCITED,BLUE};

int main() 
{
    return 0;
}
```

- 会隐式转换为int
- 用来表征枚举变量的实际类型不能明确指定，从而无法支持枚举类型的前向声明。

## 经典做法
解决作用域不受限带来的命名冲突问题的一个简单方法是，给枚举变量命名时加前缀，如上面例子改成 COLOR_BLUE 以及 FEELING_BLUE。

一般说来，为了一致性我们会把所有常量统一加上前缀。但是这样定义枚举变量的代码就显得累赘。C 程序中可能不得不这样做。不过 C++ 程序员恐怕都不喜欢这种方法。替代方案是命名空间:

``` c++
namespace Color 
{
    enum Type
    {
        RED=15,
        YELLOW,
        BLUE
    };
};
```

这样之后就可以用 Color::Type c = Color::RED; 来定义新的枚举变量了。如果 using namespace Color 后，前缀还可以省去，使得代码简化。不过，因为命名空间是可以随后被扩充内容的，所以它提供的作用域封闭性不高。在大项目中，还是有可能不同人给不同的东西起同样的枚举类型名。

更“有效”的办法是用一个类或结构体来限定其作用域，例如：定义新变量的方法和上面命名空间的相同。不过这样就不用担心类在别处被修改内容。这里用结构体而非类，一是因为本身希望这些常量可以公开访问，二是因为它只包含数据没有成员函数。

``` c++
struct Color1
{
    enum Type
    {
        RED=102,
        YELLOW,
        BLUE
    };
};

Color1.Type
```

## C++11 的枚举类
上面的做法解决了第一个问题，但对于后两个仍无能为力。庆幸的是，C++11 标准中引入了“枚举类”(enum class)，可以较好地解决上述问题。

- 新的enum的作用域不再是全局的
- 不能隐式转换成其他类型

``` c++
enum class Color2 {
  RED = 2,
  YELLOW,
  BLUE
};

Color c2 = Color2::RED;

cout << static_cast<int>(c2) << endl;
```

- 可以指定用特定的类型来存储enum

``` c++
enum class Color3:char;  // 前向声明

// 定义
enum class Color3:char 
{
    RED='r',
    BLUE
};

char c3 = static_cast<char>(Color3::RED);
```

## 类中的枚举类型C
有时我们希望某些常量只在类中有效。 由于#define 定义的宏常量是全局的，不能达到目的，于是想到实用const 修饰数据成员来实现。而const 数据成员的确是存在的，但其含义却不是我们所期望的。

const 数据成员只在某个对象生存期内是常量，而对于整个类而言却是可变的，因为类可以创建多个对象，不同的对象其 const 数据成员的值可以不同。

不能在类声明中初始化 const 数据成员。以下用法是错误的，因为类的对象未被创建时，编译器不知道 SIZE 的值是什么。(c++11标准前)

``` c++
class A 
{
  const int SIZE = 100;   // 错误，企图在类声明中初始化 const 数据成员 
  int array[SIZE];  // 错误，未知的 SIZE 
}; 
```

也许我们可以这样

``` c++
#include <iostream>

using namespace std;

class A {
public:
  A(int size); // 构造函数
  void func() { cout << SIZE << endl; }

private:
  static const int SIZE;
};

const int A::SIZE = 100;

A::A(int size) // 构造函数的定义
{}

int main() {
  A a(100); // 对象 a 的 SIZE 值为 100
  A b(200); // 对象 b 的 SIZE 值为 200
  a.func();
}

```


``` c++
class Person{
public:
    typedef enum {
        BOY = 0,
        GIRL
    }SexType;
};
//访问的时候通过，Person::BOY或者Person::GIRL来进行访问。
```

枚举常量不会占用对象的存储空间，它们在编译时被全部求值。

枚举常量的缺点是：它的隐含数据类型是整数，其最大值有限，且不能表示浮点。
# decltype那些事

## 1.基本使用¶
decltype的语法是:


decltype (expression)
这里的括号是必不可少的,decltype的作用是“查询表达式的类型”，因此，上面语句的效果是，返回 expression 表达式的类型。注意，decltype 仅仅“查询”表达式的类型，并不会对表达式进行“求值”。

### 1.1 推导出表达式类型¶

``` c++
int i = 4;
decltype(i) a; //推导结果为int。a的类型为int。

```

### 1.2 与using/typedef合用，用于定义类型。¶

``` c++
using size_t = decltype(sizeof(0));//sizeof(a)的返回值为size_t类型
using ptrdiff_t = decltype((int*)0 - (int*)0);
using nullptr_t = decltype(nullptr);
vector<int >vec;
typedef decltype(vec.begin()) vectype;
for (vectype i = vec.begin; i != vec.end(); i++)
{
//...
}
```

这样和auto一样，也提高了代码的可读性。

### 1.3 重用匿名类型¶
在C++中，我们有时候会遇上一些匿名类型，如:

``` c++
struct 
{
    int d ;
    doubel b;
}anon_s;

```

而借助decltype，我们可以重新使用这个匿名的结构体：


decltype(anon_s) as ;//定义了一个上面匿名的结构体
### 1.4 泛型编程中结合auto，用于追踪函数的返回值类型¶
这也是decltype最大的用途了。

``` c++

#include <cstddef>
#include <iostream>
#include <vector>

using namespace std;

template <typename T> auto multiply(T x, T y) -> decltype(x * y) {
  return x * y;
}

int main() {
  int nums[] = {1, 2, 3, 4};
  vector<int> vec(nums, nums + 4);
  vector<int>::iterator it;

  for (it = vec.begin(); it != vec.end(); it++) {
    cout << *it << " ";
  }
  cout << endl;

  using nullptr_t = decltype(nullptr);
  nullptr_t nu;
  int *p = NULL;
  if (p == nu) {
    cout << "NULL" << endl;
  }

  typedef decltype(vec.begin()) vectype;

  for (vectype i = vec.begin(); i != vec.end(); i++) {
    cout << *i << " ";
  }
  cout << endl;

  // 匿名结构体
  struct {
    int d;
    double b;
  } anon_s;

  decltype(anon_s) as; // 定义了一个上面匿名的结构体
  cout << multiply(11, 2) << endl;
  return 0;
}
```



# 引用和指针那些事 
## 引用与指针
|引用|指针|
|:-:|:-:|
|必须初始化|可以不初始化|
|不能为空|可以为空|
|不能更换目标|可以更换目标|
> 引用必须初始化，而指针可以不初始化。

我们在定义一个引用的时候必须为其指定一个初始值，但是指针却不需要

``` c++
int &r;    //不合法，没有初始化引用
int *p;    //合法，但p为野指针，使用需要小心
```

> 引用不能为空，而指针可以为空。

由于引用不能为空，所以我们在使用引用的时候不需要测试其合法性，而在使用指针的时候需要首先判断指针是否为空指针，否则可能会引起程序崩溃。

``` c++
void test_p(int* p)
{
    if(p != null_ptr)    //对p所指对象赋值时需先判断p是否为空指针
        *p = 3;
    return;
}
void test_r(int& r)
{
    r = 3;    //由于引用不能为空，所以此处无需判断r的有效性就可以对r直接赋值
    return;
}
```

> 引用不能更换目标

指针可以随时改变指向，但是引用只能指向初始化时指向的对象，无法改变。

``` c++
int a = 1;
int b = 2;

int &r = a;    //初始化引用r指向变量a
int *p = &a;   //初始化指针p指向变量a

p = &b;        //指针p指向了变量b
r = b;         //引用r依然指向a，但a的值变成了b
```

## 引用

### 左值引用

常规引用，一般表示对象的身份。
  
### 右值引用

右值引用就是必须绑定到右值（一个临时对象、将要销毁的对象）的引用，一般表示对象的值。

右值引用可实现转移语义（Move Sementics）和精确传递（Perfect Forwarding），它的主要目的有两个方面：

1. 消除两个对象交互时不必要的对象拷贝，节省运算存储资源，提高效率。
2. 能够更简洁明确地定义泛型函数。

<hr>

这里我们详细讲一下`右值引用`

下文引自<https://www.cnblogs.com/qicosmos/p/4283455.html>

### 从4行代码看右值引用
#### 概述
右值引用的概念有些读者可能会感到陌生，其实他和C++98/03中的左值引用有些类似，例如，c++98/03中的左值引用是这样的：

``` c++
int i = 0;
int& j = i;
```

这里的int&是对左值进行绑定（但是int&却不能绑定右值），相应的，对右值进行绑定的引用就是右值引用，他的语法是这样的A&&，通过双引号来表示绑定类型为A的右值。通过&&我们就可以很方便的绑定右值了，比如我们可以这样绑定一个右值：

``` c++
int&& i = 0;
```

这里我们绑定了一个右值0，关于右值的概念会在后面介绍。右值引用是C++11中新增加的一个很重要的特性，他主是要用来解决C++98/03中遇到的两个问题，第一个问题就是临时对象非必要的昂贵的拷贝操作，第二个问题是在模板函数中如何按照参数的实际类型进行转发。通过引入右值引用，很好的解决了这两个问题，改进了程序性能，后面将会详细介绍右值引用是如何解决这两个问题的。

和右值引用相关的概念比较多，比如：`右值`、`纯右值`、`将亡值`、`universal references`、`引用折叠`、`移动语义` 、`move语义`和`完美转发`等等。很多都是新概念，对于刚学习C++11右值引用的初学者来说，可能会觉得右值引用过于复杂，概念之间的关系难以理清。

右值引用实际上并没有那么复杂，其实是关于4行代码的故事，通过简单的4行代码我们就能清晰的理解右值引用相关的概念了。本文希望带领读者通过4行代码来理解右值引用相关的概念，理清他们之间的关系，并最终能透彻地掌握C++11的新特性--右值引用。

#### 第1行代码的故事

``` c++
int i = getVar();
```

上面的这行代码很简单，从getVar()函数获取一个整形值，然而，这行代码会产生几种类型的值呢？答案是会产生两种类型的值，一种是左值i，一种是函数getVar()返回的临时值，这个临时值在表达式结束后就销毁了，而左值i在表达式结束后仍然存在，这个临时值就是右值，具体来说是一个纯右值，右值是不具名的。区分左值和右值的一个简单办法是：看能不能对表达式取地址，如果能，则为左值，否则为右值。

所有的具名变量或对象都是左值，而匿名变量则是右值，比如，简单的赋值语句：

``` c++
int i = 0;
```

在这条语句中，i 是左值，0 是字面量，就是右值。在上面的代码中，i 可以被引用，0 就不可以了。具体来说上面的表达式中等号右边的0是纯右值（prvalue），在C++11中所有的值必属于左值、将亡值、纯右值三者之一。比如，非引用返回的临时变量、运算表达式产生的临时变量、原始字面量和lambda表达式等都是纯右值。而将亡值是C++11新增的、与右值引用相关的表达式，比如，将要被移动的对象、T&&函数返回值、std::move返回值和转换为T&&的类型的转换函数的返回值等。关于将亡值我们会在后面介绍，先看下面的代码：

``` c++
int j = 5;

auto f = []{
  return 5;
};
```

上面的代码中5是一个原始字面量， []{return 5;}是一个lambda表达式，都是属于纯右值，他们的特点是在表达式结束之后就销毁了

#### 第2行代码的故事

``` c++
T&& k = getVar();
```

第二行代码和第一行代码很像，只是相比第一行代码多了“&&”，他就是右值引用，我们知道左值引用是对左值的引用，那么，对应的，对右值的引用就是右值引用，而且右值是匿名变量，我们也只能通过引用的方式来获取右值。虽然第二行代码和第一行代码看起来差别不大，但是实际上语义的差别很大，这里，getVar()产生的临时值不会像第一行代码那样，在表达式结束之后就销毁了，而是会被“续命”，他的生命周期将会通过右值引用得以延续，和变量k的声明周期一样长。

> 右值引用的第一个特点

通过右值引用的声明，右值又“重获新生”，其生命周期与右值引用类型变量的生命周期一样长，只要该变量还活着，该右值临时量将会一直存活下去。让我们通过一个简单的例子来看看右值的生命周期。如代码清单1-1所示。

``` c++
// rvalue_examp1.cpp
#include <iostream>

using namespace std;

int g_ConstructCount = 0;
int g_CopyConstructCount = 0;
int g_destructCount = 0;

struct A {
  // 普通构造函数
  A() { cout << "construct: " << ++g_ConstructCount << endl; }

  // 拷贝构造函数
  A(const A &a) {
    cout << "copy construct: " << ++g_CopyConstructCount << endl;
  }
  ~A() { cout << "destruct: " << ++g_destructCount << endl; }
};

A GetA() {
  cout << "GetA()" << endl;
  return A(); // 先构造一个A 然后将这个临时的A 拷贝给返回值 在退出函数前销毁这个临时的A
}

int main() {
  A a = GetA(); // 将GetA() 的返回值A 拷贝给 a 然后销毁GetA()的返回值
  return 0; // 退出主函数前销毁a
}

```

``` bash
clang++ rvalue_examp1.cpp -fno-elide-constructors -o main

./main
```

从上面的例子中可以看到，在没有返回值优化的情况下，拷贝构造函数调用了两次，一次是GetA()函数内部创建的对象返回出来构造一个临时对象产生的，另一次是在main函数中构造a对象产生的。第二次的destruct是因为临时对象在构造a对象之后就销毁了。如果开启返回值优化的话，输出结果将是：

construct: 1

destruct: 1

``` c++
int main() {
  A&& a = getA();
  return 0;
}
```

``` bash
construct: 1
copy construct: 1
destruct: 1
destruct: 2
```

通过右值引用，比之前少了一次拷贝构造和一次析构，原因在于右值引用绑定了右值，让临时右值的生命周期延长了。我们可以利用这个特点做一些性能优化，即避免临时对象的拷贝构造和析构，事实上，在c++98/03中，通过常量左值引用也经常用来做性能优化。上面的代码改成：

``` bash
　　const A& a = GetA();
```


输出的结果和右值引用一样，因为常量左值引用是一个“万能”的引用类型，可以接受左值、右值、常量左值和常量右值。需要注意的是普通的左值引用不能接受右值，比如这样的写法是不对的：

``` bash
　　A& a = GetA();
```

上面的代码会报一个编译错误，因为非常量左值引用只能接受左值。

> 右值引用的第二个特点

右值引用独立于左值和右值。意思是右值引用类型的变量可能是左值也可能是右值。比如下面的例子：

``` c++
int&& var1 = 1; 
```

var1类型为右值引用，但val1本身是左值，因为具名变量都是左值。

关于右值引用一个有意思的问题是：T&&是什么，一定是右值吗？让我们来看看下面的例子：

``` c++
template<typename T>
void f(T&& t){}

f(10); //t是右值

int x = 10;
f(x); //t是左值
```

从上面的代码中可以看到，T&&表示的值类型不确定，可能是左值又可能是右值，这一点看起来有点奇怪，这就是右值引用的一个特点。

> 右值引用的第三个特点

T&& t在发生自动类型推断的时候，它是未定的引用类型（universal references），如果被一个左值初始化，它就是一个左值；如果它被一个右值初始化，它就是一个右值，它是左值还是右值取决于它的初始化。

我们再回过头看上面的代码，对于函数`template<typename T>void f(T&& t)`，当参数为右值10的时候，根据universal references的特点，t被一个右值初始化，那么t就是右值；当参数为左值x时，t被一个左值引用初始化，那么t就是一个左值。需要注意的是，仅仅是当发生自动类型推导（如函数模板的类型自动推导，或auto关键字）的时候，T&&才是universal references。再看看下面的例子：

``` c++
template<typename T>
void f(T&& param);

template<typename T>
class Test {
  Test(Test&& rhs);
}
```

上面的例子中，param是universal reference，rhs是Test&&右值引用，因为模版函数f发生了类型推断，而Test&&并没有发生类型推导，因为Test&&是确定的类型了。

正是因为右值引用可能是左值也可能是右值，依赖于初始化，并不是一下子就确定的特点，我们可以利用这一点做很多文章，比如后面要介绍的移动语义和完美转发。

这里再提一下引用折叠，正是因为引入了右值引用，所以可能存在左值引用与右值引用和右值引用与右值引用的折叠，C++11确定了引用折叠的规则，规则是这样的：

1. 所有的右值引用叠加到右值引用上仍然还是一个右值引用；
2. 所有的其他引用类型之间的叠加都将变成左值引用。


#### 第3行代码的故事

``` c++
T(T&& a) : m_val(val){ 
  a.m_val=nullptr;
}
```

#### 第4行代码的故事

``` c++
template <typename T>void f(T&& val){
  foo(std::forward<T>(val));
}
```

### 引用折叠

- X& &、X& &&、X&& & 可折叠成 X&
- X&& && 可折叠成 X&&

C++的引用在减少了程序员自由度的同时提升了内存操作的安全性和语义的优美性。比如引用强制要求必须初始化，可以让我们在使用引用的时候不用再去判断引用是否为空，让代码更加简洁优美，避免了指针满天飞的情形。除了这种场景之外引用还用于如下两个场景：

> 引用型参数

一般我们使用const reference参数作为只读形参，这种情况下既可以避免参数拷贝还可以获得与传值参数一样的调用方式。

``` c++
void test(const vector<int> &data)
{
    //...
}
int main()
{
    vector<int> data{1,2,3,4,5,6,7,8};
    test(data);
}
```


> 引用型返回值

C++提供了重载运算符的功能，我们在重载某些操作符的时候，使用引用型返回值可以获得跟该操作符原来语法相同的调用方式，保持了操作符语义的一致性。一个例子就是operator []操作符，这个操作符一般需要返回一个引用对象，才能正确的被修改。

``` c++
vector<int> v(10);
v[5] = 10;    //[]操作符返回引用，然后vector对应元素才能被修改
              //如果[]操作符不返回引用而是指针的话，赋值语句则需要这样写
*v[5] = 10;   //这种书写方式，完全不符合我们对[]调用的认知，容易产生误解

```

## 指针与引用的性能差距

指针与引用之间有没有性能差距呢？这种问题就需要进入汇编层面去看一下。我们先写一个test1函数，参数传递使用指针：

``` c++
void test1(int* p)
{
    *p = 3;    //此处应该首先判断p是否为空，为了测试的需要，此处我们没加。
    return;
}
```

该代码段对应的汇编代码如下：

``` assembly
(gdb) disassemble 
Dump of assembler code for function test1(int*):
   0x0000000000400886 <+0>:  push   %rbp
   0x0000000000400887 <+1>:  mov    %rsp,%rbp
   0x000000000040088a <+4>:  mov    %rdi,-0x8(%rbp)
=> 0x000000000040088e <+8>:  mov    -0x8(%rbp),%rax
   0x0000000000400892 <+12>: movl   $0x3,(%rax)
   0x0000000000400898 <+18>: nop
   0x0000000000400899 <+19>: pop    %rbp
   0x000000000040089a <+20>: retq   
End of assembler dump.

```

上述代码1、2行是参数调用保存现场操作；第3行是参数传递，函数调用第一个参数一般放在rdi寄存器，此行代码把rdi寄存器值（指针p的值）写入栈中；第4行是把栈中p的值写入rax寄存器；第5行是把立即数3写入到rax寄存器值所指向的内存中，此处要注意(%rax)两边的括号，这个括号并并不是可有可无的，(%rax)和%rax完全是两种意义，(%rax)代表rax寄存器中值所代表地址部分的内存，即相当于C++代码中的*p，而%rax代表rax寄存器，相当于C++代码中的p值，所以汇编这里使用了(%rax)而不是%rax。

我们再写出参数传递使用引用的C++代码段test2：

``` c++
void test2(int& r)
{
    r = 3;    //赋值前无需判断reference是否为空
    return;
}
```

这段代码对应的汇编代码如下：

``` assembly
(gdb) disassemble 
Dump of assembler code for function test2(int&):
   0x000000000040089b <+0>:  push   %rbp
   0x000000000040089c <+1>:  mov    %rsp,%rbp
   0x000000000040089f <+4>:  mov    %rdi,-0x8(%rbp)
=> 0x00000000004008a3 <+8>:  mov    -0x8(%rbp),%rax
   0x00000000004008a7 <+12>: movl   $0x3,(%rax)
   0x00000000004008ad <+18>: nop
   0x00000000004008ae <+19>: pop    %rbp
   0x00000000004008af <+20>: retq   
End of assembler dump.

```

我们发现test2对应的汇编代码和test1对应的汇编代码完全相同，这说明C++编译器在编译程序的时候将指针和引用编译成了完全一样的机器码。所以C++中的引用只是C++对指针操作的一个“语法糖”，在底层实现时C++编译器实现这两种操作的方法完全相同。

## 总结
C++中引入了引用操作，在对引用的使用加了更多限制条件的情况下，保证了引用使用的安全性和便捷性，还可以保持代码的优雅性。在适合的情况使用适合的操作，引用的使用可以一定程度避免“指针满天飞”的情况，对于提升程序稳定性也有一定的积极意义。最后，指针与引用底层实现都是一样的，不用担心两者的性能差距。

# 宏那些事

## 1.宏中包含特殊符号
分为几种：`#`，`##`，`\`

### 1.1 字符串化操作符（#）
在一个宏中的参数前面使用一个#,预处理器会把这个参数转换为一个字符数组，换言之就是：#是“字符串化”的意思，出现在宏定义中的#是把跟在后面的参数转换成一个字符串。

注意：其只能用于有传入参数的宏定义中，且必须置于宏定义体中的参数名前。

``` c++
#define exp(s) printf("test s is:%s\n",s)
#define exp1(s) printf("test s is:%s\n",#s)
#define exp2(s) #s 
int main() {
    exp("hello");
    exp1(hello);

    string str = exp2(   bac );
    cout<<str<<" "<<str.size()<<endl;
    /**
     * 忽略传入参数名前面和后面的空格。
     */
    string str1 = exp2( asda  bac );
    /**
     * 当传入参数名间存在空格时，编译器将会自动连接各个子字符串，
     * 用每个子字符串之间以一个空格连接，忽略剩余空格。
     */
    cout<<str1<<" "<<str1.size()<<endl;
    return 0;
}
```

上述代码给出了基本的使用与空格处理规则，空格处理规则如下：

- 忽略传入参数名前面和后面的空格

``` c++
string str = exp2(   bac );
cout<<str<<" "<<str.size()<<endl;
```

- 当传入参数名间存在空格时，编译器将会自动连接各个子字符串，用每个子字符串之间以一个空格连接，忽略剩余空格。

``` c++
string str1 = exp2( asda  bac );
cout<<str1<<" "<<str1.size()<<endl;
```

### 1.2 符号连接操作符（##）
**“##”是一种分隔连接方式，它的作用是先分隔，然后进行强制连接。将宏定义的多个形参转换成一个实际参数名。**

1. 当用##连接形参时，##前后的空格可有可无。
2. 连接后的实际参数名，必须为实际存在的参数名或是编译器已知的宏定义。
3. 如果##后的参数本身也是一个宏的话，##会阻止这个宏的展开。

``` c++
#define expA(s) printf("前缀加上后的字符串为:%s\n",gc_##s)  //gc_s必须存在
// 注意事项2
#define expB(s) printf("前缀加上后的字符串为:%s\n",gc_  ##  s)  //gc_s必须存在
// 注意事项1
#define gc_hello1 "I am gc_hello1"
int main() {
    // 注意事项1
    const char * gc_hello = "I am gc_hello";
    expA(hello);
    expB(hello1);
}
```

### 1.3 续行操作符（\）
当定义的宏不能用一行表达完整时，可以用”\”表示下一行继续此宏的定义。

注意 \ 前留空格。

``` c++
#define MAX(a,b) ((a)>(b) ? (a) \
   :(b))  
int main() {
    int max_val = MAX(3,6);
    cout<<max_val<<endl;
}
```

## 2.do{...}while(0)的使用


### 2.1 避免语义曲解


### 2.2避免使用goto控制流


### 2.3 避免由宏引起的警告


### 2.4 定义单一的函数块来完成复杂的操作



