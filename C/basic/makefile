CFLAGS	+=-Wall -g -lm
bin = ./bin/
src = $(wildcard *.c)
obj = $(patsubst %.c,%,$(src))

ALL:$(obj)

%:%.c
	mpicc $< -o $(bin)$@ $(CFLAGS)

clean:
	-rm -i $(bin)*

.PHONY:clean ALL
