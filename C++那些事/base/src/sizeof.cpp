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

namespace virtualInher {
//间接基类A
class A {
protected:
  int m_a;

public:
  A() { cout << sizeof(*this) << endl; }
};

//直接基类B
class B : virtual public A { //虚继承
protected:
  int m_b;

public:
  B() { cout << sizeof(*this) << endl; }
};

//直接基类C
class C : virtual public A { //虚继承
protected:
  int m_c;

public:
  C() { cout << sizeof(*this) << endl; }
};

//派生类D
class D : public B, public C {
public:
  void seta(int a) { m_a = a; } //正确
  void setb(int b) { m_b = b; } //正确
  void setc(int c) { m_c = c; } //正确
  void setd(int d) { m_d = d; } //正确
  D() { cout << sizeof(*this) << endl; }

private:
  int m_d;
};

} // namespace virtualInher

int main() {
  cout << "空类 " << sizeof(void_class) << endl;

  cout << "有虚函数 静态数据 " << sizeof(Base) << endl; // 虚指针的大小

  cout << "非空类 有非静态数据 无虚函数 无继承 " << sizeof(non_void_class)
       << endl;

  cout << "单继承(父类有虚函数) 有非静态数据 " << sizeof(Derive) << endl;

  cout << "多继承(父类有多个虚函数) 有非静态数据" << sizeof(VDerive) << endl;

  virtualInher::D vid;
  cout << "虚继承 " << sizeof(vid) << endl; // 4 + (4+8) + (4+8) + (4+8)
}
