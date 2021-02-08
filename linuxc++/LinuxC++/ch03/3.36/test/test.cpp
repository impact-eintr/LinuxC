#include <iostream>
using namespace std;
#include <stdlib.h>
#include <string.h>
 

struct movies_t {
	char title[50];
	int year;
}mine, yours;

void printmovie(movies_t movie);

int main() {
	char buffer[50];
	strcpy(mine.title, "2001 A Space Odyssey");
	mine.year = 1968;
	cout << "Enter title: ";
	cin.getline(yours.title, 50);
	cout << "Enter year: ";
	cin.getline(buffer, 50);
	yours.year = atoi(buffer);
	cout << "My favourite movie is:\n ";
	printmovie(mine);
	cout << "And yours:\n";
	printmovie(yours);
	return 0;
}

void printmovie(movies_t movie) {
	cout << movie.title;
	cout << " (" << movie.year << ")\n";
}