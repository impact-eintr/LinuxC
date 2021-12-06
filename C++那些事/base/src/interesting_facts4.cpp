#include <iostream>

using namespace std;

class Base {
protected:
  int x;

public:
  virtual void func() = 0;
  Base(int i) { x = i; }
};

class Derived : public Base {
  int y;

public:
  Derived(int i, int j) : Base(i) { y = j; }
  void func() { cout << "x = " << x << " , y = " << y; }
};

int main() {
  Derived d(4, 5);
  d.func();
  return 0;
}
