// 变量初始化

#include <iostream>
using namespace std;

int main ()
{
  int a=5;    // 初始值为 5
  int b(2);   // 初始值为 2
  int result; // 不确定初始值

  a = a + 3;
  result = a - b;
  cout << result<<endl;

  return 0;
}
