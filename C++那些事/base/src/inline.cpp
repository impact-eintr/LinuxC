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
