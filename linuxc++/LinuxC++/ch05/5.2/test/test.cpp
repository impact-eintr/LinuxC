#include <iostream>
#include <unistd.h>
using namespace std;

int main(int argc, char *argv[])
{
	pid_t pid = getpid();
	cout <<"pid="<<pid << endl;	 
	return 0;
}