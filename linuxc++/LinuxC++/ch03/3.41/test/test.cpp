#include <iostream>
using namespace std;
class CRectangle {
	int *width, *height;
public:
	CRectangle(int, int);
	~CRectangle();
	int area(void) {return (*width * *height);}
};
    
CRectangle::CRectangle(int a, int b) {
	width = new int;
	height = new int;
	*width = a;
	*height = b;
}
    
CRectangle::~CRectangle() {
	delete width;
	delete height;
}
    
int main() {
	CRectangle rect(3, 4), rectb(5, 6);
	cout << "rect area: " << rect.area() << endl;
	cout << "rectb area: " << rectb.area() << endl;
	return 0;
}