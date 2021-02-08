#include <iostream>
using namespace std;
 
int main() {
	char mybuffer[100];
	cout << "What's your name? ";
	cin.getline(mybuffer, 100);
	cout << "Hello " << mybuffer << ".\n";
	cout << "Which is your favourite team? ";
	cin.getline(mybuffer, 100);
	cout << "I like " << mybuffer << " too.\n";
	return 0;
}