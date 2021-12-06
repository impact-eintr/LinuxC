class A {
public:
  static A& getInstance();
 void  setup();

private:
  A();
  A(const A& rhs);
};

  A& A::getInstance() {
  static A a;
  return a;
}
