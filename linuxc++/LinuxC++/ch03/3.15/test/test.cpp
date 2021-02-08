#include <iostream>
using namespace std;

int main ()
{
	int n=10;
	loop:
	cout << n << ", ";
	n--;
	if (n>0) goto loop;
	cout << "FIRE!\n";
	return 0;
} 