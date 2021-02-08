#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    FILE* fp = fopen(argv[1], "r+");
    int N = atoi(argv[2]);
    char* str1 = "aaa\n";
    char* str2 = "bbbb\n";
    for (int i = 0; i < N / 2; i++) {
	fputs(str1, fp);
    }
    for (int i = N / 2; i < N; i++) {
	fputs(str2, fp);
    }
    fclose(fp);
    exit(0);
}
