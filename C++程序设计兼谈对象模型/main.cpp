#include <cstddef>
#include <iostream>
#include <string>
#include <memory>

namespace fraction {
class Fraction {
public:
  Fraction(int num, int den = 1) : m_numerator(num), m_denominator(den) {}
  operator double() const { return ((double)m_numerator / m_denominator); }

private:
  int m_numerator;   // 分子
  int m_denominator; // 分母
};

class _Fraction {
public:
  explicit _Fraction(int num, int den = 1)
      : m_numerator(num), m_denominator(den) {}
  //_Fraction operator +(const _Fraction& f) const {
  //  return ((double)m_numerator/m_denominator);
  //}
private:
  int m_numerator;   // 分子
  int m_denominator; // 分母
};

} // namespace fraction

namespace pair_tmpl {
template <class T1, class T2> struct pair {
  typedef T1 first_type;
  typedef T2 second_type;

  T1 first;
  T2 second;

  pair() : first(T1()), second(T2()) {}
  pair(const T1 &a, const T2 &b) : first(a), second(b) {}

  // 成员模板
  template <class U1, class U2>
  pair(const pair<U1, U2> &p) : first(p.first), second(p.second) {}
};

class Base1 {};
class Sub1 : public Base1 {};

class Base2 {};
class Sub2 : public Base2 {};
} // namespace pair_tmpl

namespace specialization {
template <class Key> struct hash {};

template <> struct hash<char> {
  std::size_t operator()(char x) const { return x; }
};

template <> struct hash<int> {
  std::size_t operator()(char x) const { return x; }
};

template <> struct hash<long> {
  std::size_t operator()(char x) const { return x; }
};

template <typename T, typename Alloc> class vector {};

template <typename Alloc> class vector<bool, Alloc> {};

template <typename T> class C {};

template <typename T> class C<T *> {};

template <typename T, template <typename> class SmartPtr> class XCls {
private:
  SmartPtr<T> sp;

public:
  XCls() : sp(new T) {}
};

} // namespace specialization

namespace variadic {
void print() {}

template <typename T, typename... Types>
void print(const T &firstArg, const Types &...args) {
  printf("%d\n", sizeof...(args));
  std::cout << firstArg << std::endl;
  print(args...);
}
} // namespace variadic

namespace reference {
typedef struct Stag {
  int a, b, c, d;
} S;
void referenceTest() {
  double x = 0;
  double *p = &x;
  double &r = x;

  std::cout << sizeof(x) << std::endl;
  std::cout << sizeof(p) << std::endl;
  std::cout << sizeof(r) << std::endl;

  std::cout << x << std::endl;
  std::cout << &x << std::endl;
  std::cout << p << std::endl;
  std::cout << *p << std::endl;
  std::cout << &p << std::endl;
  std::cout << r << std::endl;
  std::cout << &r << std::endl;

  S s;
  S &rs = s;

  std::cout << sizeof(s) << std::endl;
  std::cout << sizeof(rs) << std::endl;
  std::cout << &s << std::endl;
  std::cout << &rs << std::endl;
}
} // namespace reference

namespace Virtual {
  class A{
  public:
    virtual void vfunc1();
    virtual void vfunc2();
    void func1();
    void func2();

  private:
    int m_data1, m_data2;
  };

  class B : public A{
  public:
    virtual void vfunc1();
    void func2();

  private:
    int m_data3;
  };

  class C : public B{
  public:
    virtual void vfunc1();
    void func2();
  private:
    int m_data_4;
  };

}

int main(int argc, char *argv[]) {

  return 0;
}
