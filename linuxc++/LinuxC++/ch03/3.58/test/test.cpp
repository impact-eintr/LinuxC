#include <iostream>
using namespace std;
template <class Type>  
	class compare  
	{  
	public:  
		compare(Type a, Type b)  
		{  
			x = a;  
			y = b;  
		}  
		Type max()  
		{  
			return (x > y) ? x : y;  
		}  
		Type min()  
		{  
			return (x < y) ? x : y;  
		}  
	private:  
		Type x;  
		Type y;  
	};

int main(void)  
{     
	compare<int> C1(3, 5);  
	cout << "最大值：" << C1.max() << endl;  
	cout << "最小值：" << C1.min() << endl;  
          
	compare<float> C2(3.5, 3.6);  
	cout << "最大值：" << C2.max() << endl;  
	cout << "最小值：" << C2.min() << endl;  
          
	compare<char> C3('a', 'd');  
	cout << "最大值：" << C3.max() << endl;  
	cout << "最小值：" << C3.min() << endl;  
	return 0;  
}  