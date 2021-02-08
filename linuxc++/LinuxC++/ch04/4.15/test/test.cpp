#include <iostream>
#include <fstream>
using namespace std;

 
#include <stdlib.h>
    
int main() {
	char buffer[256];
	ifstream examplefile("afile.dat");
	if (!examplefile.is_open())
	{ cout << "Error opening file"; exit(1); }
	while (!examplefile.eof()) {
		examplefile.getline(buffer, 100);
		cout << buffer << endl;
	}
	return 0;
}