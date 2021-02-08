#include <iostream>  
#include <thread>  
using namespace std;  
      
void thfunc(int n)  
{  
	cout << "thfunc: " << n << "\n";   
}  
      
int main(int argc, char *argv[])  
{  
	thread t(thfunc,1);  
	t.join();  
      
	return 0;  
}  