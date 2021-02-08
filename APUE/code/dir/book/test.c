#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main()
{
    char* hello = "hello world";
    long unsigned len = strlen(hello);
    if ((hello = realloc(hello, len * 2)) == NULL) {
	perror("realloc()");
	exit(1);
    }
    //hello[len] = 0;
    printf("%s", hello);
}
