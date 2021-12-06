#include <iostream>

using namespace std;

class Apple {
private:
  int people[100];

public:
  Apple(int i);
  const int apple_number;
  void take(int num) const;
  int add(int num);
  int add(int num) const;
  int getCount() const;
};

Apple::Apple(int i) : apple_number(i) {}

int Apple::add(int num) {
  take(num);
  return 0;
}

int Apple::add(int num) const {
  take(num * 2);
  return 0;
}

void Apple::take(int num) const { cout << "take func" << num << endl; }

int Apple::getCount() const {
  take(1);
  return apple_number;
}

int main() {
  int i = 12;
  int j = 10;

  const int *ip = &i;
  ip = &j;

  int const *ip_ = &i;
  ip_ = &j;

  int *const iq = &i;
  *iq = j;

  const int *const ipq = &i;

  Apple a(2);
  cout << a.getCount() << endl;
  a.add(10);
  const Apple b(100);

  b.add(100);
  return 0;
}
