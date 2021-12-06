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
