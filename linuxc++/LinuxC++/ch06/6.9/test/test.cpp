#include <stdio.h>         
#include <stdlib.h>    
#include <string.h>  
#include <sys/stat.h>  
#include <sys/sem.h>   
int main()  
{  
        char    filename[50];  
        struct stat     buf;  
        int     ret;  
        strcpy( filename, "./test" );  
        ret = stat( filename, &buf );  
        if( ret )  
        {  
                printf( "stat error\n" );  
                return -1;  
        }  
  
        printf( "the file info: ftok( filename, 0x27 ) = %x, st_ino = %x, st_dev= %x\n", ftok( filename, 0x27 ), buf.st_ino, buf.st_dev );  
  
        return 0;  
}  