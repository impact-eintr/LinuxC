#include <iostream>
using namespace std;
 
int main() {
	int value1 = 5, value2 = 15;
	int *p1, *p2;
	p1 = &value1; // p1 = address of value1
	p2 = &value2; // p2 = address of value2
	*p1 = 10; // value pointed by p1 = 10
	*p2 = *p1; // value pointed by p2 = value pointed by p1
	p1 = p2; // p1 = p2 (value of pointer copied)
	*p1 = 20; // value pointed by p1 = 20
	cout << "value1==" << value1 << "/ value2==" << value2<<endl;
	return 0;
}