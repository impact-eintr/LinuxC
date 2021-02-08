#include <unistd.h>  

int main(int argc, char* argv[])
{
	execlp("pwd", "",  NULL); 
	return 0;
}
