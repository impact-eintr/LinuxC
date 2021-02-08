#include <iostream>
using namespace std;
void zww(int age)
{
	int a, b, c;
	if (age > 60)
		cout << "I am old\n";
	else
		cout << "I am young\n";
}
int main()
{
	int a = 5, b = 6;
	zww(70);

	a++; //line 16
	b++;
	if (a > b)
		cout << a << endl;
	else
		cout << b << endl;   
	return 0;
}


