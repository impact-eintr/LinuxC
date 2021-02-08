#include <iostream>
using namespace std;
 
void odd(int a);
void even(int a);

int main() {
	int i;
	do {
		cout << "Type a number: (0 to exit)";
		cin >> i;
		odd(i);
	} while (i != 0);
	return 0;
}

void odd(int a) {
	if ((a % 2) != 0) cout << "Number is odd.\n";
	else even(a);
}

void even(int a) {
	if ((a % 2) == 0) cout << "Number is even.\n";
	else odd(a);
}