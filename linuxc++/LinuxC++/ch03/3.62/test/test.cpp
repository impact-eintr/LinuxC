#include <iostream>
using namespace std;
namespace first {
	int var = 5;
}
    
namespace second {
	double var = 3.1416;
}
    
int main() {
	{
		using namespace first;
		cout << var << endl;
	}
	{
		using namespace second;
		cout << var << endl;
	}
	return 0;
}   