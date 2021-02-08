#include <iostream>  
#include <thread>  
 
using namespace std;  
   
void fun(int & n)  //线程函数
{  
	cout << "fun: " << n << "\n";  
	n += 20;  
	this_thread::sleep_for(chrono::milliseconds(10));   //等待10毫秒
}  
int main()  
{  
	int n = 0;  
	
	cout << "n=" << n << '\n';  
	n = 10;  
 thread t1(fun, ref(n));   //ref(n)是取n的引用
	 thread t2(move(t1));     //t2执行fun，t1不是thread对象
	 t2.join();  //等待t2执行完毕
	cout << "n=" << n << '\n';  
	return 0;  
}  
