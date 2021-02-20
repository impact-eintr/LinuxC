#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main()
{
    FILE *tmpfp;
    tmpfp = tmpfile();

    const char* oribuf = "2020/12/25";
    int SIZE = 0;
    while(*(oribuf+SIZE) != 0){
        SIZE++;
    }
    printf("%d\n",SIZE);
    
    fwrite(oribuf,1,SIZE,tmpfp);
    fseek(tmpfp,0,SEEK_SET);
    

    FILE *fp;
    fp = fopen("tmp","w");
    char desbuf[SIZE];
    int n = fread(desbuf,1,SIZE,tmpfp);
    fwrite(desbuf,1,n,fp);

    fclose(tmpfp);
    fclose(fp);
    return 0;
}

