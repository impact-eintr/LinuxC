#include <iostream>
using namespace std;
class CPolygon {
protected:
	int width, height;
public:
	void set_values(int a, int b)
	  { width = a; height = b; }
};
    
class COutput {
public:
	void output(int i);
};
    
void COutput::output(int i) {
	cout << i << endl;
}
    
class CRectangle : public CPolygon, public COutput {
public:
	int area(void)
	  { return (width * height); }
};
    
class CTriangle : public CPolygon, public COutput {
public:
	int area(void)
	  { return (width * height / 2); }
};
    
int main() {
	CRectangle rect;
	CTriangle trgl;
	rect.set_values(4, 5);
	trgl.set_values(4, 5);
	rect.output(rect.area());
	trgl.output(trgl.area());
	return 0;
}
			