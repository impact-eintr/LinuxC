##
# Mem_alloca and Mem_free
#
# @file
# @version 0.1
CC     = gcc
CFLAGS += -g
COMMON = ../common/convert.c
ALGO   = ../algorithm
SRC    = block.c small_list.c segregated_list.c mem_alloc.c main.c
IM     = $(ALGO)/linkedlist.c  implicit_list.c
EX     = $(ALGO)/linkedlist.c  explicit_list.c
RBT    = $(ALGO)/linkedlist.c  $(ALGO)/bst.c $(ALGO)/rbt.c explicit_list.c redblack_tree.c

implicit:
	$(CC) $(SRC) $(IM) -DIMPLICIT_FREE_LIST -g -o test
	./test

explicit:
	$(CC) $(SRC) $(EX) -DEXPLICIT_FREE_LIST -g -o test
	./test

rbtree:
	$(CC) $(COMMON) $(SRC) $(RBT) -DREDBLACK_TREE -g -o test
	./test

clean:
	$(RM) ./test

PHONY: clean

# end
