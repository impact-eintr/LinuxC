#include <dirent.h>
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#define PATH "/etc/*"

static int errfunc_(const char* errpath, int eerrno)
{
    puts(errpath);
    fprintf(stderr, "ERROR NESSAGE:%s\n", strerror(eerrno));
    return 0;
}

void AnalysisDir1(char* dirpath)
{
    DIR* dp;
    struct dirent* cur;

    dp = opendir(dirpath);
    if (dp == NULL) {
	perror("opendir error");
	exit(1);
    }

    while ((cur = readdir(dp)) != NULL) {
	puts(cur->d_name);
    }

    closedir(dp);
}

void AnalysisDir2()
{

    glob_t globres;
    int err;
    err = glob(PATH, 0, &errfunc_, &globres);
    if (err) {
	printf("ERROR num = %d\n", err);
	exit(1);
    }
    for (int i = 0; i < globres.gl_pathc; i++)
	puts(globres.gl_pathv[i]);
}
int main(int argc, char* argv[])
{
    AnalysisDir1("/etc");
    exit(0);
}
