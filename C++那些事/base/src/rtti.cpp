#include <iostream>
#include <typeinfo>

using namespace std;

class A {
  int a_data;

public:
  A() {} // 无参构造函数
  A(int data) : a_data(data) {} //有参构造函数
  A(const A &a) : a_data(a.a_data) {} // 拷贝构造函数
  A &operator=(const A &a) { // 拷贝赋值
    if (this == &a) {
      return *this;
    }
    a_data = a.a_data;
    return *this;
  }
  virtual ~A() {}
};

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
