##
# test file makefile
#
# @file
# @version 0.1
CC     = gcc
CFLAGS += -g -std=c11
COMMON = ../common
ALGO   = ../algorithm
ALOC   = ../malloc
CPU    = ../hardware/cpu
MEM    = ../hardware/memory
LINK   = ../linker
PROC   = ../process


# Mem_alloca and Mem_free
MSRC   = $(ALOC)/block.c $(ALOC)/small_list.c $(ALOC)/segregated_list.c \
         $(ALOC)/mem_alloc.c
IM     = $(ALGO)/linkedlist.c $(ALOC)/implicit_list.c -DIMPLICIT_FREE_LIST
EX     = $(ALGO)/linkedlist.c $(ALOC)/explicit_list.c -DEXPLICIT_FREE_LIST
RBT    = $(ALGO)/linkedlist.c  $(ALGO)/bst.c $(ALGO)/rbt.c \
				 $(ALOC)/explicit_list.c $(ALOC)/redblack_tree.c -DREDBLACK_TREE

implicit:
	$(CC) $(MSRC) $(IM) test_malloc.c  $(CFLAGS) -o test
	./test

explicit:
	$(CC) $(MSRC) $(EX) test_malloc.c  $(CFLAGS) -o test
	./test

rbtree:
	$(CC) $(COMMON)/convert.c $(MSRC) $(RBT) test_malloc.c $(CFLAGS) -o test
	./test


# ISA
ISRC    = $(ALGO)/hashtable.c $(ALGO)/trie.c \
					$(CPU)/isa.c $(CPU)/mmu.c $(CPU)/interrupt.c $(CPU)/inst.c $(CPU)/sram.c \
					$(MEM)/dram.c  $(MEM)/swap.c\
					$(PROC)/syscall.c $(PROC)/pagefault.c $(PROC)/schedule.c \
					test_run_isa.c

isa:
	$(CC) $(COMMON)/convert.c $(ISRC) $(CFLAGS) -o test
	./test


# PageFault
PSRC    = $(ALGO)/hashtable.c $(ALGO)/trie.c \
					$(CPU)/isa.c $(CPU)/mmu.c $(CPU)/interrupt.c $(CPU)/inst.c $(CPU)/sram.c \
					$(MEM)/dram.c  $(MEM)/swap.c\
					$(PROC)/syscall.c $(PROC)/pagefault.c $(PROC)/schedule.c

pagefault:
	$(CC) $(COMMON)/convert.c $(PSRC) test_pagefault.c $(CFLAGS) -o test
	./test


# Switch Context with KMALLOC(mem_alloc)
CSRC    = $(ALGO)/hashtable.c $(ALGO)/trie.c \
					$(CPU)/isa.c $(CPU)/mmu.c $(CPU)/interrupt.c $(CPU)/inst.c $(CPU)/sram.c \
					$(MEM)/dram.c  $(MEM)/swap.c\
					$(PROC)/syscall.c $(PROC)/pagefault.c $(PROC)/schedule.c

context:
	$(CC) $(COMMON)/convert.c $(CSRC) $(MSRC) $(RBT) test_context.c $(CFLAGS) -o test
	./test


# Switch Context with KMALLOC(mem_alloc)
FSRC    = $(ALGO)/hashtable.c $(ALGO)/trie.c \
					$(CPU)/isa.c $(CPU)/mmu.c $(CPU)/interrupt.c $(CPU)/inst.c $(CPU)/sram.c \
					$(MEM)/dram.c  $(MEM)/swap.c\
					$(PROC)/syscall.c $(PROC)/pagefault.c $(PROC)/schedule.c $(PROC)/fork.c $(PROC)/vmarea.c

fork:
	$(CC) $(COMMON)/convert.c $(FSRC) $(MSRC) $(IM) test_fork.c $(CFLAGS) -DUSE_PAGETABLE_VA2PA -DUSE_TLB_HARDWARE -DUSE_FORK_COW -o test
	./test


clean:
	$(RM) ./test

PHONY: clean

# end
