#include <stdio.h>



/*****************
 *sprintf atoi fprintf
 *sprintf 可以看作是 atoi的反向操作
 * **************/
int main()
{
    char buf[1024];
    int y = 2020;
    int m = 12;
    int d = 24;
    sprintf(buf,"%d/%d/%d",y,m,d);
    printf("%s",buf);
    return 0;
}

