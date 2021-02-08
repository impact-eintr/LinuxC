#include <unistd.h>  

int main(int argc, char* argv[])
{
	execl("/bin/pwd", "asdfaf",  NULL);  
	
	return 0;
}