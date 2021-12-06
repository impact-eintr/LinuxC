#include <iostream>
#include <cstdio>

using namespace std;

struct Base {
private:
  int data1;
public:
  Base(int d) {
    data1 = d;
    cout << "Base()" << endl;
  }
};

class Derived : public Base {

};

int main() {
  struct Base b(1);
}
