#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

//使用 time 计算当前单核 累加 的速度
int main()
{

    time_t end;
    int64_t count = 0;

    end = time(NULL)+5;//当前时戳+5秒
    
    while (time(NULL) <= end){
        count++;
    }

    printf("count %ld",count);
    exit(0);
}

