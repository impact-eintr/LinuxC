#include <glob.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define PATHSIZE 1024

/*
 * 当一个源程序由多个源文件组成时，非静态的全局变量在各个源文件中都是有效的。 而静态全局变量则限制了其作用域， 即只在定义该变量的源文件内有效， 在同一源程序的其它源文件中不能使用它。由于静态全局变量的作用域局限于一个源文件内，只能为该源文件内的函数公用，因此可以避免在其它源文件中引起错误
 */
static int errfunc_(const char* errpath, int eerrno)
{
    puts(errpath);
    fprintf(stderr, "ERROR NESSAGE:%s\n", strerror(eerrno));
    return 0;
}

static int path_noloop(const char* path)
{
    char* pos;
    pos = strrchr(path, '/'); //返回最后一个出现‘/’的字符指针
    if (pos == NULL)
	exit(1);

    if (strcmp(pos + 1, ".") == 0 || strcmp(pos + 1, "..") == 0) {
	return 0;
    }
    return 1;
}

void println(const char* nextpath, int size)
{

    for (int i = 0; i < size; i++) {
	printf("%c", nextpath[i]);
    }
    printf("\n");
}

static int64_t mydu(const char* path)
{
    struct stat statres;
    int sum = 0;

    if (lstat(path, &statres) < 0) {
	perror("lstat()");
	exit(1);
    }
    if (!S_ISDIR(statres.st_mode)) {
	return statres.st_blocks;
    }

    char nextpath[PATHSIZE];
    glob_t globres;
    int err, size = 0;

    strncpy(nextpath, path, PATHSIZE);
    strncat(nextpath, "/*", PATHSIZE - 1);
    size = sizeof(nextpath) / sizeof(nextpath[0]);
    println(nextpath, size);
    err = glob(nextpath, 0, &errfunc_, &globres);
    if (err) {
	printf("ERROR num = %d\n", err);
	exit(1);
    }

    strncpy(nextpath, path, PATHSIZE);
    strncat(nextpath, "/.*", PATHSIZE - 1);
    //size = sizeof(nextpath) / sizeof(nextpath[0]);
    println(nextpath, size + 1);
    err = glob(nextpath, GLOB_APPEND, &errfunc_, &globres);
    if (err) {
	printf("ERROR num = %d\n", err);
	exit(1);
    }

    sum += statres.st_blocks;

    for (int i = 0; i < globres.gl_pathc; i++) {
	if (path_noloop(globres.gl_pathv[i])) {
	    sum += mydu(globres.gl_pathv[i]);
	}
    }

    globfree(&globres);
    return sum;
}

int main(int argc, char** argv)
{
    printf("%ld\n", mydu(argv[1]) / 2);
}
