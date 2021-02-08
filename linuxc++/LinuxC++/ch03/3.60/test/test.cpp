#include <iostream>
using namespace std;
namespace first {
	int var = 5;
}
    
namespace second {
	double var = 3.1416;
}
    
int main() {
	cout << first::var << endl;
	cout << second::var << endl;
	return 0;
}   