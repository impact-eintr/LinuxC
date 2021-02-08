src = $(wildcard *.c)
obj = $(patsubst %.c,%,$(src))

ALL:$(obj)

%:%.c
	gcc $< -o $@ -g -lpthread -Wall

clean:
	-rm -rf $(obj)

.PHONY:clean ALL
