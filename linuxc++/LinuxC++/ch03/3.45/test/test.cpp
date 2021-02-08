#include <iostream>
using namespace std;
   
class CDummy {
public:
	int isitme(CDummy& param);
};
    
int CDummy::isitme(CDummy& param) {
	if (&param == this) return 1;
	else return 0;
}
    
int main() {
	CDummy a;
	CDummy* b = &a;
	if (b->isitme(a))
		cout << "yes, &a is b\n";
	return 0;
}