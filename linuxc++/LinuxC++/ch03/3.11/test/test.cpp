#include <iostream>
using namespace std;
int main ()
{
	unsigned long n;
	do {
	cout << "Enter number (0 to end): ";
	cin >> n;
	cout << "You entered: " << n << "\n";
	} while (n != 0);
	return 0;
} 