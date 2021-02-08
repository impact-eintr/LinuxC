#include "string.h"
#include "iostream"
using namespace std;

int main(int argc, char* argv[])
{
	char sz1[] = "a世界a1a都去asdfad哪啦";
	string str;
	int i, len = strlen(sz1); //得到字符数组长度

	for (int i = 0; i < len;)
	{
		if (sz1[i] < 0)   //负数则前后两个字节存的是汉字
		{
			str.push_back(sz1[i]); 
			i++; 
			str.push_back(sz1[i]);
		}
		i++;
	}
	cout << str << endl; //输出找到的汉字

	return 0;
}
