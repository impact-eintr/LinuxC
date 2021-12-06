#include <iostream>
#include <stddef.h>
#include <stdlib.h>
#include <string>

class foo {
public:
  int _id;
  long _data;
  std::string _str;

public:
  foo() : _id(0) {
    std::cout << "调用default ctor.this=" << this << "id=" << _id << std::endl;
  }
  foo(int i) : _id(i) {
    std::cout << "调用default ctor.this=" << this << "id=" << _id << std::endl;
  }
  //virtual ~foo() { std::cout << "调用dtor.this=" << this << "id=" << _id << std::endl; }
  ~foo() { std::cout << "调用dtor.this=" << this << "id=" << _id << std::endl; }

  static void *operator new(size_t size);
  static void operator delete(void *pdead, size_t size);
  static void *operator new[](size_t size);
  static void operator delete[](void *pdead, size_t size);
};

void *foo::operator new(size_t size) {
  std::cout << "调用new() size=" << size <<std::endl;
  std::cout << "先申请内存\n";
  foo *p = (foo *)malloc(size);
  return p;
}

void foo::operator delete(void *pdead, size_t size) {
  std::cout << "调用delete()\n";
  std::cout << "free内存\n";
  free(pdead);
}

void *foo::operator new[](size_t size) {
  std::cout << "调用new[]() size=" << size <<std::endl;
  std::cout << "先申请内存\n";
  foo *p = (foo *)malloc(size);
  return p;
}

void foo::operator delete[](void *pdead, size_t size) {
  std::cout << "调用delete[]()\n";
  std::cout << "free内存\n";
  free(pdead);
}

int main(int argc, char *argv[]) {

  std::cout << sizeof(foo) << std::endl;

  foo *p = new foo(7);
  delete p;

  foo *parray = new foo[5];
  delete[] parray;

  return 0;
}
