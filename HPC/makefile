CFLAGS	+=-Wall -g -O2
bin = ./bin/
src = $(wildcard *.c)
obj = $(patsubst %.c,%,$(src))

ALL:$(obj)

%:%.c
	mpicc $< -o $(bin)$@ $(CFLAGS)

clean:
	-rm -i $(bin)*

.PHONY:clean ALL
