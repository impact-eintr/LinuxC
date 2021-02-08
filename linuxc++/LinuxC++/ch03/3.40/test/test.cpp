#include <iostream>
using namespace std;
class CRectangle {
	int width, height;
public:
	CRectangle(int, int);
	int area(void) {return (width*height);}
};
    
CRectangle::CRectangle(int a, int b) {
	width = a;
	height = b;
}
    
int main() {
	CRectangle rect(3, 4);
	CRectangle rectb(5, 6);
	cout << "rect area: " << rect.area() << endl;
	cout << "rectb area: " << rectb.area() << endl;
}    
		