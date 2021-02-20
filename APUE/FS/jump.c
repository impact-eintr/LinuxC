#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

static jmp_buf save;

static void d(){
    printf("%s is called\n",__FUNCTION__);
    longjmp(save,2);
    printf("%s is returned\n",__FUNCTION__);
}


static void c(){
    printf("%s is called\n",__FUNCTION__);
    d();
    printf("%s is returned\n",__FUNCTION__);
}


static void b(){
    printf("%s is called\n",__FUNCTION__);
    c();
    printf("%s is returned\n",__FUNCTION__);
}

static void a(){
    int ret = setjmp(save);
    if  (ret == 0) {
        printf("%s is called\n",__FUNCTION__);
        b();
        printf("%s is returned\n",__FUNCTION__);
    }else {
        printf("code %d return \n",ret);
    }
}

int main()
{
    a();
    return 0;
}

