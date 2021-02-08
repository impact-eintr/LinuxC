#include <iostream>  
#include <thread>  
using namespace std;  
   
void thfunc(int n,int m,int *k,char s[])  
{  
	cout << "in thfunc:n=" <<n<<",m="<<m<<",k="<<*k<<"\nstr="<<s<<endl;  
	*k = 5000;
}  
      
int main(int argc, char *argv[])  
{  
	int n = 110,m=200,k=5;
	char str[] = "hello world";

	thread t(thfunc, n,m,&k,str);   
	t.join();   
	cout << "k=" << k << endl;

	return 0;  
}  
