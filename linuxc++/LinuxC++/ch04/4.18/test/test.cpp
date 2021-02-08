#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

int main() {
	struct stat buf;
	stat("/etc/hosts", &buf);
	printf("/etc/hosts file size = %d\n", buf.st_size);
	
	stat("/zww/test/myfile.txt", &buf);
	printf("/zww/test/myfile.txt size = %d\n", buf.st_size);
}