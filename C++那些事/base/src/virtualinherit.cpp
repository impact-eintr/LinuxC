#include <iostream>

using namespace std;

namespace test1 {
class A {
  int k1;
  int k2;

public:
  virtual void aa(){};
};

class B : public virtual A {
  int j;

public:
  virtual void bb(){};
};

class C : public virtual B {
  int i;

public:
  virtual void cc(){};
};
} // namespace test1

namespace test2 {
struct A {
  int ax; // 成员变量
  virtual void f0() {}
  virtual void f1() {}
};

struct B : public A {
  int bx;               // 成员变量
  void f0() override{}; // 重写f0
};
} // namespace test2

int main() {
  // cout << sizeof(test1::A) << endl; // 4 + 4 + 8(vptr)
  // cout << sizeof(test1::B) << endl; // 4(B) + 8(vptr) + 8(vptr to A) + 4(A) +
  // 8(vptr) cout << sizeof(test1::C) << endl; // 4(B) + 8(vptr) + 8(vptr to B)
  // + 4(B) +
  cout << sizeof(test2::A) << endl;
  cout << sizeof(test2::B) << endl;
  return 0;
}
