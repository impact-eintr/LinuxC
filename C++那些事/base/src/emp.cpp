#include <iostream>

using namespace std;

class Employee {
public:
  virtual void raiseSalary() { cout << 0 << endl; }
  virtual void promote() {}
};

class Manager : public Employee {
  virtual void raiseSalary() { cout << 100 << endl; }
  virtual void promote() {}
};

class Engineer : public Employee {
  virtual void raiseSalary() { cout << 200 << endl; }
  virtual void promote() {}
};

void globalRaiseSalary(Employee *emp[], int n) {
  for (int i = 0;i < n;i++) {
    emp[i]->raiseSalary();
  }
}

int main() {
  Employee *emp[] = {
    new Manager(),
    new Engineer
  };
  globalRaiseSalary(emp, sizeof(emp)/sizeof(*emp));
  return 0;

}
