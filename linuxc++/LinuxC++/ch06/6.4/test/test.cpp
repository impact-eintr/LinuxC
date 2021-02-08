#include <stdio.h>
#include <signal.h>
int main(int argc, char *argv[]) 
{
	signal(SIGINT, SIG_IGN);
	while (1);
	return 0;
}