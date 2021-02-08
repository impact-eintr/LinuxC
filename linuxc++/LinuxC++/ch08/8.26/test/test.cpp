#include <iostream>  
#include <thread>  
using namespace std;  
void thfunc(int n,int m,int *k,char s[])  //线程函数
{  
	cout << "in thfunc:n=" <<n<<",m="<<m<<",k="<<*k<<"\nstr="<<s<<endl;  
	*k = 5000;
}  
      
int main(int argc, char *argv[])  
{  
	int n = 110,m=200,k=5;
	char str[] = "hello world";

	thread t(thfunc, n,m,&k,str);   //定义线程对象
	t.detach();  //分离线程
	
	cout << "k=" << k << endl;  //这里输出3
	pthread_exit(NULL); //main线程结束，但进程并不会结束，下面一句不会执行
	
	cout << "this line will not run"<< endl;  //这一句不会执行
	return 0;  
}  
