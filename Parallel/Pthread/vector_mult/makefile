CFLAGS		+=-Wall -g -lpthread -O2
bin = ./bin/
src = $(wildcard *.c)
obj = $(patsubst %.c,%,$(src))

ALL:$(obj)

%:%.c
	gcc $< -o $(bin)$@ $(CFLAGS)

clean:
	-rm -i $(bin)*

.PHONY:clean ALL
