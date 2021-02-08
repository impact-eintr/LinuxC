#include <iostream>  
#include <thread>  
using namespace std;  
      
void thfunc(char *s)  //线程函数
{  
	cout << "thfunc: " <<s << "\n";   //这里s就是boy and girl
}  
      
int main(int argc, char *argv[])  
{  
	char s[] = "boy and girl"; //定义一个字符串
	thread t(thfunc,s);  //定义线程对象，并传入字符串s
	t.join();   //等待t执行结束
      
	return 0;  
}  
