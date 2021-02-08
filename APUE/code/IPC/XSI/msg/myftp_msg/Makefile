src = $(wildcard *.c)
obj = $(patsubst %.c,%,$(src))

ALL:$(obj)

%:%.c
	gcc $< -o $@ -g -Wall

clean:
	-rm -rf $(obj)

.PHONY:clean ALL
