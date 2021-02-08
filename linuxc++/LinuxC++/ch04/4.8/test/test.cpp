#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include <iostream>
using namespace std;

int main(void)
{
	struct stat st; 
    
	if (-1 == stat("/root/test.txt", &st))  
	{   
		cout << ("stat failed\n");
		return -1; 
	}   
	cout<<"file length:"<<st.st_size<<"byte"<<endl; 
	cout << "mod time:" << st.st_mtime << endl;  
	cout << "node:" << st.st_ino << endl;  
	cout << "mode:" << st.st_mode << endl;  
}
