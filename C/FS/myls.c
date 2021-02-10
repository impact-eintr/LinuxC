#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <glob.h>
#include <string.h>
#include <dirent.h>

#include "myls.h"

#define PAT "/etc/a*.conf"

static struct stat statres;

//文件大小
static off_t flen(const char *fname){
    if (stat(fname,&statres) < 0) {
        perror("stat()");
        exit(1);
    }
    return statres.st_size;
}

//文件类型
static char ftype(const char* fname) {
    if (stat(fname,&statres) < 0) {
        perror("stat()");
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
/**
enum
  {
    DT_UNKNOWN = 0,
# define DT_UNKNOWN	DT_UNKNOWN
    DT_FIFO = 1,
# define DT_FIFO	DT_FIFO
    DT_CHR = 2,
# define DT_CHR		DT_CHR
    DT_DIR = 4,
# define DT_DIR		DT_DIR
    DT_BLK = 6,
# define DT_BLK		DT_BLK
    DT_REG = 8,
# define DT_REG		DT_REG
    DT_LNK = 10,
# define DT_LNK		DT_LNK
    DT_SOCK = 12,
# define DT_SOCK	DT_SOCK
    DT_WHT = 14
# define DT_WHT		DT_WHT
  };
*/
#define VNAME(value) (#value)
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
        fprintf(stdout,"%s type:%d inode:%ld\n",cur->d_name,cur->d_type,cur->d_ino);
    }

    closedir(dp);
}

int main(int argc,char **argv)
{
    if (argc < 2) {
        fprintf(stderr,"Usage...\n");
        exit(1);
    }

    int c = 0;
    fileAttr f;
    char fmtstr[FMTSTRSIZE];
    fmtstr[0] = 0;
    

    //解析命令行
    while(1) {
        c = getopt(argc,argv,"lt-a"); // - 识别非选项的传参
        if (c < 0){
            break;
        }
        
        switch (c){
            case 'l':
                f.filesize = flen(argv[1]);
                strncat(fmtstr,"filesize:%ld ",FMTSTRSIZE-1);
                break;
            case 't':
                f.filetype = ftype(argv[1]);
                strncat(fmtstr,"filetype:%c ",FMTSTRSIZE-1);
                break;
            case 'a':
                PathParse(argv[optind]);
                break;
            default:
                break;
        }
    }

    printf(fmtstr,f.filesize,f.filetype);
    //char pwd[1024];
    //getcwd(pwd,1024);
    //fprintf(stdout,"%s\n",pwd);
    //Glob();
    //PathParse(pwd);

    exit(0);
}

