#include <stdlib.h>
#include <stdio.h>

int main(){
    printf("fileno(stdin) = %d\t%d\t%d\n",fileno(stdin),fileno(stdout),fileno(stderr));
    exit(0);
}
