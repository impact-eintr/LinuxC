#include <iostream>

using namespace std;

class Test {
  int x;

public:
  virtual void show() = 0;
  int getX() { return x; }
};

int main() {
  Test t; // error
  return 0;
}
