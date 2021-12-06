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
