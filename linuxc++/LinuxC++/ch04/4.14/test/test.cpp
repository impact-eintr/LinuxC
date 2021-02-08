#include <iostream>
#include <fstream>
using namespace std;

    
const char * filename = "afile.dat";
    
int main() {
	long l, m;
	ifstream file(filename, ios::in | ios::binary);
	l = file.tellg();
	file.seekg(0, ios::end);
	m = file.tellg();
	file.close();
	cout << "size of " << filename;
	cout << " is " << (m - l) << " bytes.\n";
	return 0;
}
