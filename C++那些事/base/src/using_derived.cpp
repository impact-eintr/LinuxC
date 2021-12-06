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
