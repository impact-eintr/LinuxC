#include <iostream>
using namespace std;
 
long factorial(long a) {
	if (a > 1) return (a * factorial(a - 1));
	else return (1);
}

int main() {
	long l;
	cout << "Type a number: ";
	cin >> l;
	cout << "!" << l << " = " << factorial(l)<<endl;
	return 0;
}
