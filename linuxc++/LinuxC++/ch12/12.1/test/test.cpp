#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	int nNum = 0x12345678;
	char *p = (char*)&nNum;  //p指向存储nNum的内存的低地址
	 
	if (*p == 0x12) cout << "This machine is big endian." << endl; //判断低地址是否存放的是数据高位
	else cout << "This machine is small endian." << endl;   
  
	return 0;
}
