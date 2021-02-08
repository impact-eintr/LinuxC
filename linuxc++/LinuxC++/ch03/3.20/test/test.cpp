#include <iostream>
using namespace std;
 
void prevnext(int x, int& prev, int& next)
{
	prev = x - 1;
	next = x + 1;
}

int main()
{
	int x = 100, y, z;
	prevnext(x, y, z);
	cout << "Previous=" << y << ", Next=" << z<<endl;
	return 0;
}