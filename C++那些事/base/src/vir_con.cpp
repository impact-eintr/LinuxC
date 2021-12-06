#include <iostream>

using namespace std;

class Base {
public:
  Base() {}

  virtual ~Base() {}

  virtual void ChangeAttributes() = 0;

  // "Virtual Constructor"
  static Base *Create(int id);

  // The "Virtual Copy Constructor"
  virtual Base *Clone() = 0;
};

class Derived1 : public Base {
public:
  Derived1() { cout << "Derived1 created" << endl; }

  Derived1(const Derived1 &rhs) {
    cout << "Derived1 created by deep copy" << endl;
  }

  ~Derived1() { cout << "~Derived1 destroyed" << endl; }

  void ChangeAttributes() { cout << "Derived1 Attributes Changed" << endl; }

  Base *Clone() { return new Derived1(*this); }
};

class Derived2 : public Base {

public:
  Derived2() { cout << "Derived2 created" << endl; }

  Derived2(const Derived2 &rhs) {
    cout << "Derived2 created by deep copy" << endl;
  }

  ~Derived2() { cout << "~Derived2 destroyed" << endl; }

  void ChangeAttributes() { cout << "Derived2 Attributes Changed" << endl; }

  Base *Clone() { return new Derived2(*this); }
};

class Derived3 : public Base {
public:
  Derived3() { cout << "Derived3 created" << endl; }

  Derived3(const Derived3 &rhs) {
    cout << "Derived3 created by deep copy" << endl;
  }

  ~Derived3() { cout << "~Derived3 destroyed" << endl; }

  void ChangeAttributes() { cout << "Derived3 Attributes Changed" << endl; }

  Base *Clone() { return new Derived3(*this); }
};

Base *Base::Create(int id) {
  if (id == 1) {
    return new Derived1;
  } else if (id == 2) {
    return new Derived2;
  } else {
    return new Derived3;
  }
}

class User {
public:
  User() : pBase(nullptr) {
    int input;
    cout << "Enter ID (1, 2 or 3): ";
    cin >> input;

    while ((input != 1) && (input != 2) && (input != 3)) {
      cout << "Enter ID (1, 2 or 3 only): ";
      cin >> input;
    }

    // Create objects via the "Virtual Constructor"
    pBase = Base::Create(input);
  }

  ~User() {
    if (pBase) {
      delete pBase;
      pBase = nullptr;
    }
  }

  void Action() {
    Base *pNewBase = pBase->Clone();
    pNewBase->ChangeAttributes();
    delete pNewBase;
  }

private:
  Base *pBase;
};

int main() {
  User *user = new User();

  user->Action();

  delete user;
}
