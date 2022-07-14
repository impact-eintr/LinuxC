int main() {
  __asm__(
    // hello world\n\0
    "movq $0x000000000a646c72, %rbx \n" \
    "pushq %rbx \n" \
    "movq $0x6f77206f6c6c6568, %rbx \n" \
    "pushq %rbx \n" \
    // syscall : write
    "movq $1, %rax \n" \
    "movq $1, %rdi \n" \
    "movq %rsp, %rsi \n" \
    "movq $13, %rdx \n" \
    "syscall \n" \
    // syscall : exit
    "movq $60, %rax \n" \
    "movq $0, %rdi \n" \
    "syscall \n" \
    );
}
