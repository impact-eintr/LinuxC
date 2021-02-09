#include <linux/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <glob.h>
#include <string.h>
#include <dirent.h>

#define PAT "/etc/a*.conf"

static struct stat statres;

//文件大小
static off_t flen(const char *fname){
    if (stat(fname,&statres) < 0) {
        perror("tata()");
        exit(1);
    }
    return statres.st_size;
}

//文件类型
static int ftype(const char* fname) {
    if (stat(fname,&statres) < 0) {
        perror("rstat()");
        exit(1);
    }
    if (S_ISREG(statres.st_mode)) {
        return '-';
    }else if (S_ISDIR(statres.st_mode)) {
        return 'd';
    }else{
        return '?';
    }
}

//解析失败处理函数
static int errfunc (const char *errpath,int eerrno) {
    puts(errpath);
    fprintf(stderr,"ERROR MSG: %s\n",strerror(eerrno));
    return 0;
}


//glob解析路径
static void Glob(){
    glob_t globres;
    int err = glob(PAT,0,&errfunc,&globres);
    if (err) {
        printf("Error code = %d\n",err);
    }

    for (int i = 0;globres.gl_pathv[i]!= NULL;i++) {
        fprintf(stdout,"%s\n",globres.gl_pathv[i]);
    }
}

//组合解析路径
static void PathParse(char *Path) {
    DIR *dp;
    struct dirent *cur;

    dp = opendir(Path);
    if (dp == NULL) {
          perror("opendir");
          exit(1);
    }

    while((cur = readdir(dp)) != NULL) {
        fprintf(stdout,"%s ",cur->d_name);
        fprintf(stdout,"type:%d ",cur->d_type);
    }

    closedir(dp);
}


int main(int argc,char **argv)
{
    //if (argc < 2) {
    //    fprintf(stderr,"Usage...\n");
    //    exit(1);
    //}

    //long len = flen(argv[1]);
    //int type = ftype(argv[1]);
    //printf("%c",type);
    //printf(" file_size = %ld\n",len);

    char pwd[1024];
    getcwd(pwd,1024);
    fprintf(stdout,"%s\n",pwd);
    Glob();
    PathParse(pwd);

    exit(0);
}

