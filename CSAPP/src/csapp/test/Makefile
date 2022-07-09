##
# test file makefile
#
# @file
# @version 0.1
CC     = gcc
CFLAGS += -g -Wall
COMMON = ../common
ALGO   = ../algorithm
ALOC   = ../malloc
CPU    = ../hardware/cpu
MEM    = ../hardware/memory
LINK   = ../linker
PROC   = ../process


# Mem_alloca and Mem_free
MSRC   = $(ALOC)/block.c $(ALOC)/small_list.c $(ALOC)/segregated_list.c \
         $(ALOC)/mem_alloc.c test_malloc.c
IM     = $(ALGO)/linkedlist.c $(ALOC)/implicit_list.c
EX     = $(ALGO)/linkedlist.c $(ALOC)/explicit_list.c
RBT    = $(ALGO)/linkedlist.c  $(ALGO)/bst.c $(ALGO)/rbt.c \
				 $(ALOC)/explicit_list.c $(ALOC)/redblack_tree.c

implicit:
	$(CC) $(MSRC) $(IM) -DIMPLICIT_FREE_LIST $(CFLAGS) -o test
	./test

explicit:
	$(CC) $(MSRC) $(EX) -DEXPLICIT_FREE_LIST $(CFLAGS) -o test
	./test

rbtree:
	$(CC) $(COMMON)/convert.c $(MSRC) $(RBT) -DREDBLACK_TREE $(CFLAGS) -o test
	./test

# ISA
ISRC    = $(ALGO)/hashtable.c $(ALGO)/trie.c \
					$(CPU)/isa.c $(CPU)/mmu.c $(CPU)/interrupt.c $(CPU)/inst.c $(CPU)/sram.c \
					$(MEM)/dram.c  $(MEM)/swap.c\
					$(PROC)/syscall.c $(PROC)/pagefault.c $(PROC)/schedule.c \
					test_run_isa.c

isa:
	$(CC) $(COMMON)/convert.c $(ISRC) $(CLAGS) -o test
	./test

clean:
	$(RM) ./test

PHONY: clean

# end
