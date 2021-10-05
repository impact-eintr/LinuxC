# OS
欢迎来到操作系统第一课。在真正打造操作系统前，有一条必经之路：你知道程序是如何运行的吗？一个熟练的编程老手只需肉眼看着代码，就能对其运行的过程了如指掌。
但对于初学者来说，这常常是很困难的事，这需要好几年的程序开发经验，和在长期的程序开发过程中对编程基本功的积累。
我记得自己最初学习操作系统的时候，面对逻辑稍微复杂的一些程序，在编写、调试代码时，就会陷入代码的迷宫，找不到东南西北。
不知道你现在处在什么阶段，是否曾有同样的感受？我常常说，扎实的基本功就像手里的指南针，你可以一步步强大到不依赖它，但是不能没有。
因此今天，我将带领你从“Hello World”起，扎实基本功，探索程序如何运行的所有细节和原理

``` c++
#include "stdio.h"
int main(int argc, const char **argv) {
    printf("Hello world\n");
    return 0;
}
```

计算机硬件是无法直接运行这个 C 语言文本程序代码的，需要 C 语言编译器，把这个代码编译成具体硬件平台的二进制代码。再由具体操作系统建立进程，把这个二进制文件装进其进程的内存空间中，才能运行。

![img](https://static001.geekbang.org/resource/image/f2/4a/f2b10135ed52436888a793327e4d5a4a.jpg)

其实，我们也可以手动控制以上这个编译流程，从而留下中间文件方便研究：
- gcc HelloWorld.c -E -o HelloWorld.i 预处理：加入头文件，替换宏。
- gcc HelloWorld.c -S -c HelloWorld.s 编译：包含预处理，将 C 程序转换成汇编程序。
- gcc HelloWorld.c -c HelloWorld.o 汇编：包含预处理和编译，将汇编程序转换成可链接的二进制程序。
- gcc HelloWorld.c -o HelloWorld 链接：包含以上所有操作，将可链接的二进制程序和其它别的库链接在一起，形成可执行的程序文件。

图灵机是一个抽象的模型，它是这样的：有一条无限长的纸带，纸带上有无限个小格子，小格子中写有相关的信息，纸带上有一个读头，读头能根据纸带小格子里的信息做相关的操作并能来回移动。

![img](https://static001.geekbang.org/resource/image/69/7d/6914497643dbb0aaefffc32b865dcf7d.png)

![img](https://static001.geekbang.org/resource/image/43/87/43812abfe104d6885815825f07622e87.jpg)

根据冯诺依曼体系结构构成的计算机，必须具有如下功能：
- 把程序和数据装入到计算机中；
- 必须具有长期记住程序、数据的中间结果及最终运算结果；
- 完成各种算术、逻辑运算和数据传送等数据加工处理；
- 根据需要控制程序走向，并能根据指令控制机器的各部件协调操作；
- 能够按照要求将处理的数据结果显示给用户。

为了完成上述的功能，计算机必须具备五大基本组成部件：
- 装载数据和程序的输入设备；
- 记住程序和数据的存储器；
- 完成数据加工处理的运算器；
- 控制程序执行的控制器；
- 显示处理结果的输出设备

![img](https://static001.geekbang.org/resource/image/bd/26/bde34df011c397yy42dc00fe6bd35226.jpg)

是不是非常简单？这次我们发现读头不再来回移动了，而是靠地址总线寻找对应的“纸带格子”。读取写入数据由数据总线完成，而动作的控制就是控制总线的职责了。

![img](https://static001.geekbang.org/resource/image/39/14/3991a042107b90612122b14596c65614.jpeg)

![img](https://static001.geekbang.org/resource/image/5d/6e/5d4889e7bf20e670ee71cc9b6285c96e.jpg)

重点回顾以上，对应图中的伪代码你应该明白了：现代电子计算机正是通过内存中的信息（指令和数据）做出相应的操作，并通过内存地址的变化，达到程序读取数据，控制程序流程（顺序、跳转对应该图灵机的读头来回移动）的功能。
这和图灵机的核心思想相比，没有根本性的变化。只要配合一些 I/O 设备，让用户输入并显示计算结果给用户，就是一台现代意义的电子计算机。
到这里，我们理清了程序运行的所有细节和原理。还有一点，你可能有点疑惑，即 printf 对应的 puts 函数，到底做了什么？而这正是我们后面的课程要探索的！

![img](https://static001.geekbang.org/resource/image/f2/bd/f2d31ab7144bf309761711efa9d6d4bd.jpg?wh=4335*3170)

简单解释一下，PC 机 BIOS 固件是固化在 PC 机主板上的 ROM 芯片中的，掉电也能保存，PC 机上电后的第一条指令就是 BIOS 固件中的，它负责检测和初始化 CPU、内存及主板平台，然后加载引导设备（大概率是硬盘）中的第一个扇区数据，到 0x7c00 地址开始的内存空间，再接着跳转到 0x7c00 处执行指令，在我们这里的情况下就是 GRUB 引导程序。

我们先来写一段汇编代码。这里我要特别说明一个问题：为什么不能直接用 C？
C 作为通用的高级语言，不能直接操作特定的硬件，而且 C 语言的函数调用、函数传参，都需要用栈。
栈简单来说就是一块内存空间，其中数据满足后进先出的特性，它由 CPU 特定的栈寄存器指向，所以我们要先用汇编代码处理好这些 C 语言的工作环境。

``` assembly
;eintr
mbt_hdr_flags equ 0x00010003
mbt_hdr_magic equ 0x1badb002  ; 多协议引导头魔数
mbt_hdr2_magic equ 0xe85250d6  ; 第二版多协议引导头魔数
global _start ; 导出_start符号
extern main; 导出外部的main函数的符号
[section .start.text] ;定义.start.text代码段
[bits 32] ;汇编成32未代码
_start;
  jmp _entry
align 8
mbt_hdr:
  dd mbt_hdr_magic
  dd mbt_hdr_flags
  dd -(mbt_hdr_magic+mbt_hdr_flags)
  dd mbt_hdr
  dd _start
  dd 0
  dd 0
  dd _entry
; 以上是grub所需要的头
align 8
mbt2_hdr:
  dd mbt_hdr2_magic
  dd 0
  dd mbt2_hdr_end - mbt2_hdr
  dd -(mbt_hdr2_magic + 0 + (mbt2_hdr_end - mbt2_hdr))
  dw 2, 0
  dd 24
  dd mbt2_hdr
  dd _start
  dd 0
  dd 0
  dw 3, 0
  dd 12
  dd _entry
  dd 0
  dw 0, 0
  dd 8
mbt2_hdr_end:
;以上是grub2所需要的头
align 8

_entry:
	;关中断
	cli
	;关不可屏蔽中断
	in al, 0x70
	or al, 0x80
	out 0x70,al
	;重新加载gdt
	lgdt [gdt_ptr]
	jmp dword 0x8 :_32bits_mode

_32bits_mode:
	;下面初始化c语言可能会用到的寄存器
	mov ax, 0x10
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	xor eax,eax
	xor ebx,ebx
	xor ecx,ecx
	xor edx,edx
	xor edi,edi
	xor esi,esi
	xor ebp,ebp
	xor esp,esp
	;初始化栈，c语言需要栈才能工作
	mov esp,0x9000
	;调用c语言函数main
	call main
	;让cpu停止执行指令
halt_step:
	halt
	jmp halt_step


gdt_start:
knull_dsc: dq 0
kcode_dsc: dq 0x00cf9e000000ffff
kdata_dsc: dq 0x00cf92000000ffff
k16cd_dsc: dq 0x00009e000000ffff
k16da_dsc: dq 0x000092000000ffff
gdt_end:

gdt_ptr:
gdtlen	dw gdt_end-gdt_start-1
gdtbase	dd gdt_start

```

以上的汇编代码（/lesson01/HelloOS/entry.asm）分为 4 个部分：
1. 代码 1~40 行，用汇编定义的 GRUB 的多引导协议头，其实就是一定格式的数据，我们的 Hello OS 是用 GRUB 引导的，当然要遵循 GRUB 的多引导协议标准，让 GRUB 能识别我们的 Hello OS。之所以有两个引导头，是为了兼容 GRUB1 和 GRUB2。
2. 代码 44~52 行，关掉中断，设定 CPU 的工作模式。你现在可能不懂，没事儿，后面 CPU 相关的课程我们会专门再研究它。
3. 代码 54~73 行，初始化 CPU 的寄存器和 C 语言的运行环境。
4. 代码 78~87 行，GDT_START 开始的，是 CPU 工作模式所需要的数据，同样，后面讲 CPU 时会专门介绍。

### 控制计算机屏幕
接着我们再看下显卡，这和我们接下来要写的代码有直接关联。
计算机屏幕显示往往是显卡的输出，显卡有很多形式：集成在主板的叫集显，做在 CPU 芯片内的叫核显，独立存在通过 PCIE 接口连接的叫独显，性能依次上升，价格也是。
独显的高性能是游戏玩家们所钟爱的，3D 图形显示往往要涉及顶点处理、多边形的生成和变换、纹理、着色、打光、栅格化等。而这些任务的计算量超级大，所以独显往往有自己的 RAM、多达几百个运算核心的处理器。因此独显不仅仅是可以显示图像，而且可以执行大规模并行计算，比如“挖矿”。
我们要在屏幕上显示字符，就要编程操作显卡。其实无论我们 PC 上是什么显卡，它们都支持一种叫 VESA 的标准，这种标准下有两种工作模式：字符模式和图形模式。显卡们为了兼容这种标准，不得不自己提供一种叫 VGABIOS 的固件程序。
下面，我们来看看显卡的字符模式的工作细节。
它把屏幕分成 24 行，每行 80 个字符，把这（24*80）个位置映射到以 0xb8000 地址开始的内存中，每两个字节对应一个字符，其中一个字节是字符的 ASCII 码，另一个字节为字符的颜色值。如下图所示：

![img](https://static001.geekbang.org/resource/image/78/f5/782ef574b96084fa44a33ea1f83146f5.jpg?wh=3530*1605)

``` c++
#ifndef VGASTR_H_
#define VGASTR_H_

static inline void _strwrite(char* string) {
  char *p_strdst = (char*)(0xb8000); // 指向显存的开始地址
  while(*string) {
    *p_strdst = *string++;
    p_strdst += 2;
  }
  return;
}

inline void printf(char* fmt, ...) {
  _strwrite(fmt);
  return;
}

#endif // VGASTR_H_

```


``` c++
#include "vgastr.h"

int main(int argc, char **argv) {
  printf("Hello OS!");
  return 0;
}

```

下面我们用一张图来描述我们 Hello OS 的编译过程，如下所示：
![img](https://static001.geekbang.org/resource/image/cb/34/cbd634cd5256e372bcbebd4b95f21b34.jpg?wh=4378*4923)
