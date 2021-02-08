#include <unistd.h>  
using namespace std;
#include <iostream>

int main(int argc, char* argv[])
{
	execl("mytest", NULL); //不传任何参数给mytest
	cout << "------------------\n";//如果execl执行成功，这一句不会执行到的。
	
	return 0;
}
