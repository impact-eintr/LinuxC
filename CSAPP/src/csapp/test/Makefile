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
SRC    = $(ALOC)/block.c $(ALOC)/small_list.c $(ALOC)/segregated_list.c $(ALOC)/mem_alloc.c test_malloc.c
IM     = $(ALGO)/linkedlist.c $(ALOC)/implicit_list.c
EX     = $(ALGO)/linkedlist.c $(ALOC)/explicit_list.c
RBT    = $(ALGO)/linkedlist.c  $(ALGO)/bst.c $(ALGO)/rbt.c $(ALOC)/explicit_list.c $(ALOC)/redblack_tree.c

implicit:
	$(CC) $(SRC) $(IM) -DIMPLICIT_FREE_LIST -g -o test
	./test

explicit:
	$(CC) $(SRC) $(EX) -DEXPLICIT_FREE_LIST -g -o test
	./test

rbtree:
	$(CC) $(COMMON)/convert.c $(SRC) $(RBT) -DREDBLACK_TREE -g -o test
	./test

clean:
	$(RM) ./test

PHONY: clean

# end
