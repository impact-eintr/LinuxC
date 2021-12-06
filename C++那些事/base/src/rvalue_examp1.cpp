#include <iostream>

using namespace std;

int g_ConstructCount = 0;
int g_CopyConstructCount = 0;
int g_destructCount = 0;

struct A {
  // 普通构造函数
  A() { cout << "construct: " << ++g_ConstructCount << endl; }

  // 拷贝构造函数
  A(const A &a) {
    cout << "copy construct: " << ++g_CopyConstructCount << endl;
  }
  ~A() { cout << "destruct: " << ++g_destructCount << endl; }
};

A GetA() {
  cout << "GetA()" << endl;
  return A(); // 先构造一个A 然后将这个临时的A 拷贝给返回值 在退出函数前销毁这个临时的A
}

int main() {
  A&& a = GetA(); // 将GetA() 的返回值A 拷贝给 a 然后销毁GetA()的返回值
  return 0; // 退出主函数前销毁a
}
