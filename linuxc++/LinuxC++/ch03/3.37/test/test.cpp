#include <iostream>
using namespace std;
#include <stdlib.h>
#include <string.h>
 

#define N_MOVIES 5

struct movies_t {
	char title[50];
	int year;
} films[N_MOVIES];

void printmovie(movies_t movie);

int main() {
	char buffer[50];
	int n;
	for (n = 0; n < N_MOVIES; n++) {
		cout << "Enter title: ";
		cin.getline(films[n].title, 50);
		cout << "Enter year: ";
		cin.getline(buffer, 50);
		films[n].year = atoi(buffer);
	}

	cout << "\nYou have entered these movies:\n";
	for (n = 0; n < N_MOVIES; n++)  
		printmovie(films[n]);
	return 0;
}

	void printmovie(movies_t movie) {
		cout << movie.title;
		cout << " (" << movie.year << ")\n";
	}