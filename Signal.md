# 信号

## 信号的概念
**信号是软件中断，横多比较重要的应用程序都需要处理信号，信号提供了一种处理异步时间的方法**
**信号的响应依赖于中断**
- kill -l
## signal()
- `void (*signal(int signum,void (*func)(int)))(int)`

~~~ c
void test(int sig){};

void (*signal(int sig,test))(int){};
//void (*res)(int) signal(int sig,test){};

~~~

~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define N 20

void handler(int sig){
    write(1,"!",1);
}


int main()
{
    int i;

    signal(SIGINT,handler);

    for (i = 0;i < N;i++){
        write(1,"*",1);
        sleep(1);
    }
    
    exit(0);
}
~~~


**信号会打断阻塞的系统调用**

## 信号的不可靠
**信号的行为不可靠**

## 可重入函数
**解决信号的不可靠问题 第一次调用没结束 第二次调用开始了 **
**第一次调用没结束 发生第二次调用 但不会出错**
**所有的系统调用都是可重入的 一部分库函数 **
## 信号的响应过程(重要)
**信号从收到到响应有不可避免的延时**

**如何忽略掉一个信号，标准信号为什么要丢失**

## 信号的常用函数
- kill() **不是用来杀死进程的，是用来发送信号的，只不过大多数信号有杀死进程的作用**

~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>

int main()
{
    //int kill(pid_t pid, int sig)
    //pid > 0 发送给指定进程
    //pid == 0 发送给同组所有进程 即 组内广播
    //pid == -1 全局广播 init专用
    //pid < -1 发送给 ppid == |pid|的所有进程
    //当sig == 0时用于检测一个进程是否存在


    exit(0);
}
~~~

- raise() 等价与`kill(getpid(), sig);`
- **alarm()**
- pause 人为制造的阻塞系统调用 等待一个信号来打断它

~~~ c
//5sec_sig.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

//使用 信号 计算当前单核 累加 的速度

static int loop = 1;

static void handler(int sig){
    loop = 0;
}

int main()
{
    int64_t count = 0;

    alarm(5);
    signal(SIGALRM,handler);

    while(loop){
        count++;
    }

    printf("count %ld",count);

    exit(0);
}

~~~

~~~ bash
gcc 5sec_sig.c -S
~~~

~~~ x86asm
main:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	$0, -8(%rbp)
	movl	$5, %edi
	call	alarm@PLT
	leaq	handler(%rip), %rsi
	movl	$14, %edi
	call	signal@PLT
	jmp	.L3
.L4:
	addq	$1, -8(%rbp)
.L3:
	movl	loop(%rip), %eax ;执行循环
	testl	%eax, %eax
	jne	.L4
	movq	-8(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %edi
	call	exit@PLT
	.cfi_endproc
~~~


#### volatile 关键字
**去到这个变量真正的存储空间取数值，而不是根据局部结构判断取值**

~~~ bash
gcc 5sec_sig.c -O1
./a.out         //不会有返回的 进入死循环

~~~

注意注释
~~~ x86asm
main:
.LFB23:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$5, %edi
	call	alarm@PLT
	leaq	handler(%rip), %rsi
	movl	$14, %edi
	call	signal@PLT
	cmpl	$0, loop(%rip)
	je	.L3
.L4:
	jmp	.L4
.L3:
	movl	$0, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %edi
	call	exit@PLT
	.cfi_endproc
.LFE23:
	.size	main, .-main
	.data
	.align 4
	.type	loop, @object
	.size	loop, 4
loop:
	.long	1;将loop的值固定为1
	.ident	"GCC: (GNU) 10.2.0"
	.section	.note.GNU-stack,"",@progbits
~~~

原因在于这一句
~~~ c
//loop在局部循环中没有改变值 gcc 认为loop的值不会发生改变 永远为 1
while(loop){
    count++;
}
~~~

解决办法就是
~~~ c
static volatile int loop = 1;
~~~

#### 漏桶(流控算法)
~~~ c

~~~

#### 令牌桶(优化后的流控算法)
~~~ c

~~~

- abort
- system()

#### sleep的缺陷

## 信号集

## 信号屏蔽字/pending集的处理


## 扩展
- sigsuspend()
- sigaction()
- setitimer()

## 实时信号
