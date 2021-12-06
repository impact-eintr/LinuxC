#include <iostream>
using namespace std;

class apple {
public:
  static int i;

  apple(){
      // do nothing
  };
};

int apple::i = 1;

int main() {
  apple obj1;
  apple obj2;

  cout << apple::i << endl;

  obj1.i = 2;
  cout << obj1.i << endl;

  obj2.i = 3;
  cout << obj2.i << endl;
}
