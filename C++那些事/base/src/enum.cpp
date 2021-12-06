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
