/*彭东 @ 2021.01.09*/

ENTRY(_start)
OUTPUT_ARCH(i386)
OUTPUT_FORMAT(elf32-i386)
SECTIONS
{
	. = 0x200000;
	__begin_start_text = .;
	.start.text : ALIGN(4) { *(.start.text) }
	__end_start_text = .;

	__begin_text = .;
	.text : ALIGN(4) { *(.text) }
	__end_text = .;

	__begin_data = .;
	.data : ALIGN(4) { *(.data) }
	__end_data = .;

	__begin_rodata = .;
	.rodata : ALIGN(4) { *(.rodata) *(.rodata.*) }
	__end_rodata = .;

	__begin_kstrtab = .;
	.kstrtab : ALIGN(4) { *(.kstrtab) }
	__end_kstrtab = .;

	__begin_bss = .;
       .bss : ALIGN(4) { *(.bss) }
	__end_bss = .;
}
