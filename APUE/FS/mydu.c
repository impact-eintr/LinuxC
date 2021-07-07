#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <glob.h>
#include <string.h>

#define PATHSIZE 1024

static int path_noloop(const char *path) {
    char *pos;
    pos = strrchr(path,'/');
    if (pos == NULL) {
        exit(1);
    }

    if (strcmp(pos+1,".") == 0||strcmp(pos+1,"..")== 0) {
        return 0;
    }
    return 1;
}

static int64_t mydu(const char *path) {
    static struct stat statres;
    static char nextpath[PATHSIZE];
    glob_t globres;
    int64_t sum = 0;

    //非目录
    if (lstat(path,&statres) < 0) {
        perror("lstat()");
        exit(1);
    }

    if (!S_ISDIR(statres.st_mode)){
        fprintf(stdout,"%ld\t%s\n",statres.st_blocks / 2,path);
        return statres.st_blocks;
    }
    //目录
    //拼接路径
    strncpy(nextpath,path,PATHSIZE);
    strncat(nextpath,"/*",PATHSIZE);
    if (glob(nextpath,0,NULL,&globres) < 0) {
        fprintf(stderr,"glob()");
        exit(1);
    }

    strncpy(nextpath,path,PATHSIZE);
    strncat(nextpath,"/.*",PATHSIZE);
    if (glob(nextpath,GLOB_APPEND,NULL,&globres) < 0) {
        fprintf(stderr,"glob()");
        exit(1);
    }

    sum = statres.st_blocks;
    for (int i = 0;i < globres.gl_pathc;i++){
        if (path_noloop(globres.gl_pathv[i]))
            sum += mydu(globres.gl_pathv[i]);
    }
    
    globfree(&globres);//回收资源
    return sum;
        
}

int main(int argc,char **argv)
{   
    if (argc < 2) {
        fprintf(stderr,"%s\n","Usage...");
        exit(1);
    }
    
    printf("%ld\t%s\n",mydu(argv[1])/2,argv[1]);

    return 0;
}

