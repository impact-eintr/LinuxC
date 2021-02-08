#include "apue.h"
#include <unistd.h>
int main(void)
{
    if (lseek(STDIN_FILENO, 0, SEEK_CUR) == -1) {
	printf("不能置偏移位\n");
    } else
	printf("seek ok\n");
    exit(0);
}
