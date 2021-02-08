#include <string.h>
using namespace std;
#include <iostream>
 
int main(int argc, char* argv[])
{
	int i;
	cout <<"argc=" << argc << endl; //打印下传进来的参数个数
	
	for(i=0;i<argc;i++)   //打印各个参数
		cout<<argv[i]<<endl;
}
