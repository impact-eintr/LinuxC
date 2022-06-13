##
# Mem_alloca and Mem_free
#
# @file
# @version 0.1
CC     = gcc
CFLAGS += -g
SRC    = block.c small_list.c segregated_list.c mem_alloc.c main.c
IM     = ../algorithm/linkedlist.c  implicit_list.c
EX     = ../algorithm/linkedlist.c  explicit_list.c
RB     = ../algorithm/linkedlist.c  explicit_list.c

implicit:
	$(CC) $(SRC) $(IM) -DIMPLICIT_FREE_LIST -g -o test
	./test

explicit:
	$(CC) $(SRC) $(EX) -DEXPLICIT_FREE_LIST -g -o test
	./test

rbtree:

clean:
	$(RM) ./test

PHONY: clean

# end
