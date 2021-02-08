#include "apue.h"
#include <dirent.h>
#include <limits.h>
#include <stdlib.h>

typedef int Myfunc(const char*, const struct stat*, int);

static Myfunc myfunc;
//myfiletypecount
static int myftw(char*, Myfunc*);
static int dopath(Myfunc*);
static long nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot;

//主函数
int main(int argc, char** argv)
{
    int ret;

    if (argc != 2)
	err_quit("usage:  ftw  <starting-pathname>");

    ret = myftw(argv[1], myfunc);

    ntot = nreg + ndir + nblk + nchr + nfifo + nslink + nsock;
    if (ntot == 0)
	ntot = 1; //防止除法错误
    printf("regular files  = %7ld, %5.2f %%\n", nreg,
	nreg * 100.0 / ntot);
    printf("directories    = %7ld, %5.2f %%\n", ndir,
	ndir * 100.0 / ntot);
    printf("block special  = %7ld, %5.2f %%\n", nblk,
	nblk * 100.0 / ntot);
    printf("char special   = %7ld, %5.2f %%\n", nchr,
	nchr * 100.0 / ntot);
    printf("FIFOs          = %7ld, %5.2f %%\n", nfifo,
	nfifo * 100.0 / ntot);
    printf("symbolic links = %7ld, %5.2f %%\n", nslink,
	nslink * 100.0 / ntot);
    printf("sockets        = %7ld, %5.2f %%\n", nsock,
	nsock * 100.0 / ntot);
    exit(ret);
}

#define FTW_F 1	  /* file other than directory */
#define FTW_D 2	  /* directory */
#define FTW_DNR 3 /* directory that can't be read */
#define FTW_NS 4  /* file that we can't stat */

static char* fullpath; /* contains full pathname for every file */
static size_t pathlen;

//myfw 计数函数
static int myftw(char* pathname, Myfunc* func)
{
    fullpath = path_alloc(&pathlen);

    if (pathlen <= strlen(pathname)) {
	pathlen = strlen(pathname) * 2;
    }

    if ((fullpath = realloc(fullpath, pathlen)) == NULL) {
	perror("realloc()");
	exit(1);
    }

    strcpy(fullpath, pathname);
    return (dopath(func));
}

//dopath
static int dopath(Myfunc* func)
{
    struct stat statbuf;
    struct dirent* dirp;
    DIR* dp;

    if (lstat(fullpath, &statbuf) < 0) {
	return (func(fullpath, &statbuf, FTW_NS));
    }
    if (S_ISDIR(statbuf.st_mode) == 0) {
	return (func(fullpath, &statbuf, FTW_F));
    }

    int ret = 0;
    int n = 0;
    if ((ret = func(fullpath, &statbuf, FTW_D)) != 0)
	return (ret);

    n = strlen(fullpath);
    if (n + NAME_MAX + 2 > pathlen) {
	pathlen *= 2;
    }

    if ((fullpath = realloc(fullpath, pathlen)) == NULL) {
	perror("realloc()");
	exit(1);
    }
    fullpath[n++] = '/';
    fullpath[n] = 0;

    if ((dp = opendir(fullpath)) == NULL) {
	return (func(fullpath, &statbuf, FTW_DNR));
    }
}

//myfunc
static int myfunc(const char* pathname, const struct stat* statptr, int type)
{
    switch (type) {
    case FTW_F:
	switch (statptr->st_mode & S_IFMT) {
	case S_IFREG:
	    nreg++;
	    break;
	case S_IFBLK:
	    nblk++;
	    break;
	case S_IFCHR:
	    nchr++;
	    break;
	case S_IFIFO:
	    nfifo++;
	    break;
	case S_IFLNK:
	    nslink++;
	    break;
	case S_IFSOCK:
	    nsock++;
	    break;
	case S_IFDIR: /* directories should have type = FTW_D */
	    err_dump("for S_IFDIR for %s", pathname);
	}
	break;
    case FTW_D:
	ndir++;
	break;
    case FTW_DNR:
	err_ret("can't read directory %s", pathname);
	break;
    case FTW_NS:
	err_ret("stat error for %s", pathname);
	break;
    default:
	err_dump("unknown type %d for pathname %s", type, pathname);
    }
    return (0);
}
