#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <map>
#include <ostream>
#include <utility>
#include <vector>

using namespace std;

#include <tuple>
namespace VariadicTemplates {
// 递归打印
void printX() {} // 参数为0的时候

template <typename T, typename... Types>
void printX(const T &firstArg, const Types &...args) {
  cout << firstArg << endl;
  printX(args...);
}

// Max/Min
int maximum(int n) { return n; }

template <typename... Args> int maximum(int n, Args... args) {
  return max(n, maximum(args...));
}

// 模拟tuple内部实现
template <typename... Values> class _tuple; // 这里表示_tuple是一个模板类

template <> class _tuple<> {
public:
  _tuple() {}
};

template <typename Head, typename... Tail> //不希望产生实参个数为0的变长参数模板
class _tuple<Head, Tail...> : private _tuple<Tail...> //继承自参数少一个的tuple
{
  typedef _tuple<Tail...> inherited;

protected:
  Head m_head;

public:
  _tuple() = default;
  _tuple(Head v, Tail... vtail) : m_head(v), inherited(vtail...) {}

  auto head() -> decltype(m_head) { return m_head; } //使用decltype进行类型推导
  // Head head(){ return m_head; }
  inherited &tail() { return *this; } // return后转换为inherited
};

template <int IDX, int MAX, typename... Args> struct PRINT_TUPLE {

  static void print(ostream &os, const tuple<Args...> &t) {
    os << get<IDX>(t) << (IDX + 1 == MAX ? "" : ",");
    PRINT_TUPLE<IDX + 1, MAX, Args...>::print(os, t); //递归调用
  }
};

// 偏特化
template <int MAX, typename... Args> struct PRINT_TUPLE<MAX, MAX, Args...> {
  static void print(ostream &os, const tuple<Args...> &t) {}
};

template <typename... Args>
ostream &operator<<(ostream &os, const tuple<Args...> &t) {
  os << "[";
  PRINT_TUPLE<0, sizeof...(Args), Args...>::print(os, t);
  return os << "]";
}

template <typename... Value> class tup;
template <> class tup<> {};
template <typename Head, typename... Tail> class tup<Head, Tail...> {
  typedef tup<Tail...> composited;

protected:
  composited m_tail;
  Head m_head;

public:
  tup() {}
  tup(Head v, Tail... vtail) : m_tail(vtail...), m_head(v) {}
  Head head() { return m_head; }
  composited &tail() { return m_tail; }
};
}; // namespace VariadicTemplates

namespace Initializer_list {
void print(initializer_list<int> vals) {
  for (auto p = vals.begin(); p != vals.end(); ++p) {
    cout << *p << "\n";
  }
}

class P {
public:
  P(int a, int b) { cout << "P(int, int), a=" << a << ", b=" << b << endl; }

  P(initializer_list<int> initlist) {
    cout << "P(initializer_list<int>), value=";
    for (auto i : initlist) {
      cout << i << " ";
    }
    cout << "\n";
  }
};

}; // namespace Initializer_list

namespace Explicit {
class P {
public:
  P(int a, int b) { cout << "P(int a, int b)" << endl; }
  P() { cout << "P()" << endl; }

  P(initializer_list<int>) { cout << "P(initializer_list<int>)" << endl; }

  explicit P(int a, int b, int c) {
    cout << "explicit P(int a, int b, int c)" << endl;
  }
};

void fp(const P &){};
}; // namespace Explicit

namespace Default {
class Zoo {
public:
  Zoo(int i1, int i2) : d1(i1), d2(i2) {}
  Zoo() : d1(0), d2(0) {}
  Zoo(const Zoo &) = delete;
  Zoo(Zoo &&) = default;
  Zoo &operator=(const Zoo &) = default;
  Zoo &operator=(const Zoo &&) = delete;
  virtual ~Zoo() {}

private:
  int d1, d2;
};
}; // namespace Default

namespace temptempparam {
#define SIZE 3000000
template <typename T, template <class> class Container> class XCls {
private:
  Container<T> c;

public:
  XCls() { // ctors
    for (long i = 0; i < SIZE; ++i) {
      c.insert(c.end(), T());
    }
    cout << "构造完毕" << endl;
  }
};
}; // namespace temptempparam

template <typename T> using Vec = vector<T, allocator<T>>;

namespace Decltype {

template <typename T1, typename T2> auto add(T1 x, T2 y) -> decltype(x + y);

auto test() -> void {
  vector<int> v;
  decltype(v)::value_type elem;
  cout << elem << endl;
}

template <typename T> void test_decltype(T obj) {
  typedef typename decltype(obj)::value_type iType;
}
}; // namespace Decltype

namespace Lambdas {
void Lambdas_test() {
  int id = 0;
  auto f = [id]() { cout << "id: " << id << endl; };
  id = 42;
  f();
  f();
  f();
  cout << id << endl;
}
}; // namespace Lambdas

namespace RValue {
template <typename M> void test_moveable(M c, long &value) {
  char buf[10];

  typedef typename iterator_traits<typename M::iterator>::value_type Vtype;
  clock_t timeStart = clock();
  for (long i = 0; i < value; ++i) {
    snprintf(buf, 10, "%d", rand());
    auto ite = c.end();
    c.insert(ite, Vtype(buf));
  }
  cout << "mill-seconds: " << (clock() - timeStart) << endl;
  cout << "size()=" << c.size() << endl;
}

void process(int &i) { cout << "Process(int&):" << i << endl; }

void process(int &&i) { cout << "Process(int&&):" << i << endl; }

void forward(int &&i) {
  cout << "forward(int&&):" << i << endl;
  process(i);
}
} // namespace RValue

#include <cstring>
namespace MoveClass {
class MyString {
public:
  static size_t DCtor; // 累计default-ctor调用次数
  static size_t Ctor;  // 累计ctor调用次数
  static size_t CCtor; // 累计copy-ctor调用次数
  static size_t CAsgn; // 累计copy-asgn调用次数
  static size_t MCtor; // 累计move-ctor调用次数
  static size_t MAsgn; // 累计move-asgn调用次数
  static size_t Dtor;  // 累计dtor调用次数
private:
  char* _data;
  size_t _len;
  void _init_data(const char *s) {
    _data = new char[_len+1];
    memcpy(_data, s, _len);
    _data[_len] = '\0';
  }
public:
  // DCtor
  MyString() :_data(nullptr), _len(0) {++DCtor;}
  // Ctor
  MyString(const char *p) : _len(strlen(p)) {
    ++Ctor;
    _init_data(p);
  }
  // CCtor
  MyString(const MyString& str) :_len(str._len) {
    ++CCtor;
    _init_data(str._data);
  }

  // MCtor
  MyString(MyString&& str) noexcept : _data(str._data), _len(str._len) {
    ++MCtor;
    str._len = 0;
    str._data = nullptr;
  }

  // CAsgn
  MyString& operator=(const MyString& str) {
    ++CAsgn;
    // 自我赋值检查
    if (this != &str) {

    } else {

    }
    return *this;
  }

};
} // namespace MoveClass

int main(int argc, char *argv[]) {
  int a = 0;
  RValue::process(a);
  RValue::process(1);
  RValue::process(move(a));

  RValue::forward(2); // Rvalue通过forward()传递给另一个函数却变成了Lvalue
                      // 这个过程中变成了named object
  RValue::forward(move(a));
  return 0;
}
