CFLAGS	+=-Wall -g -lstdc++ -lpthread -D_FILE_OFFSET_BITS=64 -D_GUN_SOURCE  -lcrypt -O2
bin = ./bin/
src = $(wildcard *.c)
obj = $(patsubst %.c,%,$(src))

ALL:$(obj)

%:%.c
	gcc $< -o $(bin)$@ $(CFLAGS)

clean:
	-rm -i $(bin)*

.PHONY:clean ALL
