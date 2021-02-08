#include <iostream>
using namespace std;
int main ()
{
	int n;
	cout << "Enter the starting number > ";
	cin >> n;
	while (n>0) {
	cout << n << ", ";
	--n;
	}
	cout << "FIRE!\n";
	return 0;
} 