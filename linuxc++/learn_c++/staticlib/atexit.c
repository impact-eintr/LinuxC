#include "apue.h"
#include "staticlibs.h"   //包含静态库的头文件

static void my_exit1(void);
static void my_exit2(void);

int main(void)
{
    static_lib_func_add(1, 9); //静态库函数
    static_lib_func_mul(1, 9); //静态库函数

    if (0 != atexit(my_exit2))
        err_sys("can't register my_exit2");
    if (0 != atexit(my_exit1))
        err_sys("can't register my_exit1");
    if (0 != atexit(my_exit1))
        err_sys("can't register my_exit1");
    printf("main is done\n");
    return 0;
}

static void my_exit1(void)
{
    printf("first exit handler\n");
}

static void my_exit2()
{
    printf("second exit handler\n");
}
