#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <glob.h>
#include <string.h>

#define PATHSIZE 1024

static int64_t mydu(const char *path) {
    struct stat statres;
    char nextpath[PATHSIZE];
    glob_t globres;

    //非目录
    if (lstat(path,&statres) < 0) {
        perror("lstat()");
        exit(1);
    }

    if (!S_ISDIR(statres.st_mode)){
        fprintf(stdout,"%ld\t%s\n",statres.st_blocks / 2,path);
        return statres.st_blocks / 2;
    }
    //目录
    //拼接路径
    strncpy(nextpath,path,PATHSIZE);
    strncat(nextpath,"/*",PATHSIZE);

    if (glob(nextpath,GLOB_APPEND,NULL,&globres) < 0) {
        fprintf(stderr,"glob()");
        exit(1);
    }


    strncpy(nextpath,path,PATHSIZE);
    strncat(nextpath,"/.*",PATHSIZE);

    if (glob(nextpath,GLOB_APPEND,NULL,&globres) < 0) {
        fprintf(stderr,"glob()");
        exit(1);
    }
    int sum = 0;
    for (int i = 0;i < globres.gl_pathc;i++){
        sum += mydu(globres.gl_pathv[i]);
    }
        
}

int main(int argc,char **argv)
{   
    if (argc < 2) {
        fprintf(stderr,"%s\n","Usage...");
        exit(1);
    }
    
    mydu(argv[1]);

    return 0;
}

