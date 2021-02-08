#include <iostream>
using namespace std;
void increase(void* data, int type) {
	switch (type) {
	case sizeof(char) : (*((char*)data))++; break;
	case sizeof(short): (*((short*)data))++; break;
	case sizeof(long) : (*((long*)data))++; break;
	}
}

int main() {
	char a = 5;
	short b = 9;
	long c = 12;
	increase(&a, sizeof(a));
	increase(&b, sizeof(b));
	increase(&c, sizeof(c));
	cout << (int) a << ", " << b << ", " << c<<endl;
	return 0;
}