CFLAGS		+=-Wall -g -lstdc++ -ldl -D_FILE_OFFSET_BITS=64 -D_GUN_SOURCE
bin = ./bin/
src = $(wildcard *.c)
obj = $(patsubst %.c,%,$(src))

ALL:$(obj)

%:%.c
	gcc $< -o $(bin)$@ -g $(CFLAGS)

clean:
	-rm -i $(bin)*

.PHONY:clean ALL
