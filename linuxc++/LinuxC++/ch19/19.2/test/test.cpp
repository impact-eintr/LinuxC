#include <iostream>
using namespace std;
#include "openssl/evp.h"  //包含相关openssl头文件，位于/usr/local/ssl/include/openssl/evp.h
int main(int argc, char *argv[])
{
	char sz[] = "Hello, OpenSSL!";	
	cout << sz << endl;
	OpenSSL_add_all_algorithms();  //载入所有SSL算法，这个函数是openssl库中的函数
	return 0;
}
