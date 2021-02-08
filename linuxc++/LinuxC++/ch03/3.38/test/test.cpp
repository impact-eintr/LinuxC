#include <iostream>
using namespace std;
#include <stdlib.h>
struct movies_t {
	char title[50];
	int year;
};

int main() {
	char buffer[50];

	movies_t amovie;
	movies_t * pmovie;
	pmovie = & amovie;

	cout << "Enter title: ";
	cin.getline(pmovie->title, 50);
	cout << "Enter year: ";

	cin.getline(buffer, 50);
	pmovie->year = atoi(buffer);

	cout << "\nYou have entered:\n";
	cout << pmovie->title;
	cout << " (" << pmovie->year << ")\n";

	return 0;
}