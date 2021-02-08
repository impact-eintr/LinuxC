## makefile
### 命名
- makefile
- Makefile
### 三个注意点
#### 一个规则
- 目标的时间必须晚于依赖条件的时间，否则更新目标
- 依赖条件如果不存在，找需新的规则去产生依赖条件
- ALL:指定makefile的最终目标
	~~~makefile
	目标:依赖条件
		命令
	~~~

#### 两个函数
- `src = $(wildcard ./*.c)`:匹配当前工作目录下的所有.c文件组成列表，赋值给变量src
	- src = add.c sub.c div.c
- `obj = $(patsubst %.c,%.o,$(src))`:将参数3中，包含参数1的部分，替换为参数2
	- obj = add.c sub.c div.c
##### clean
~~~makedile
clean:(没有依赖)
<tab>-rm -rf $(obj) a.out
#"-"的作用是删除不存在文件时，不报错。顺序执行结束
~~~
#### 三个自动变量
##### $@
- 在规则的**命令**中，表示规则中的目标
##### $^
- 在规则的命令中，表示所有依赖条件
##### $<
- 在规则的命令中，表示第一个依赖条件
- 如果将该变量套用到模式规则中，它可将依赖条件列表中的依赖依次取出，套用模式规则
##### 模式规则
~~~makefile
%.o:%.PICTURE:
	gcc -c $< -o %@
~~~
###### 结果展示
~~~:makefile
src = $(sildcard *.c)
obj = $(patsubst %.c,%.o,$(src))

ALL:a.out

a.out: $(obj)
	gcc $^ -o $@
%./o:%.c
	gcc -c $< -o %@

clean:
	rm -rf $(obj) a.out
~~~
##### 静态模式规则
~~~makefile
$(obj):%.o:%.c
gcc -c $< -o %@
~~~
##### 伪目标
~~~makefile
.PHONY:clean ALL
~~~
##### 参数
~~~makefile
-n 模拟执行make make clean指令
-f 指定文件执行make 命令
~~~