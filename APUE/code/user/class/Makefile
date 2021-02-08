src = $(wildcard *.c)
obj = $(patsubst %.c,%,$(src))

ALL:$(obj)

%:%.c
	gcc $< -o $@ -g -Wall -D_FILE_OFFSET_BITS=64 -lcrypt

clean:
	-rm -rf $(obj)

.PHONY:clean ALL
