#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc,char **argv)
{
    if (argc < 3){
        fprintf(stderr,"Usage:%s <src_file_name> <dest_file_name>\n",argv[0]);
        exit(1);
    }
    FILE *fps,*fpd;
    fps = fopen(argv[1],"r");
    if (fps ==  NULL){
        perror("fopen()");
        exit(1);
    }

    fpd = fopen(argv[2],"w");
    if (fpd == NULL){
        fclose(fps);//日后可以用钩子函数释放内存
        perror("fopen()");
        exit(1);
    }

    int res = 0;
    while(1){
        res = fgetc(fps);
        if (res == EOF){
            break;
        }
        fputc(res,fpd);
    }

    fclose(fps);
    fclose(fpd);
    return 0;
}

