#include <iostream>
using namespace std;

int main ()
{
	int n;
	for (n=10; n>0; n--)
	{
		cout << n << ", ";
		if (n==3)
		{
			cout << "countdown aborted!\n";
			break;
		}
	}
	return 0;
} 