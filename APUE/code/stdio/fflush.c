#include <stdio.h>
#include <stdlib.h>
/*
 * 缓冲区的作用 大多数时候是好事 合并系统调用
 * 行缓冲 换行时刷新 满了的时候刷新 强制刷新
 * 全缓冲 满了的时候刷新 强制刷新 (非终端的设备默认)
 * 无缓冲 需要立即输出的内容
 */
int main(void)
{
    printf("test");
    fflush(stdout);
    while (1)
	;
    printf("test");
    exit(1);
}
