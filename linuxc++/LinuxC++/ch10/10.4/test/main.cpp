extern void f1(int age);   //声明要使用的函数
extern void f2(int age);   //声明要使用的函数
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	f1(65);
	f2(66);
	cout << "bye" << endl;
	
	return 0;
}
