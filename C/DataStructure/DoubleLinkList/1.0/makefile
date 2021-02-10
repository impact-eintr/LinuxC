CFLAGS		+=-Wall -g -lstdc++ -D_FILE_OFFSET_BITS=64
CC			=gcc
TARGET		=DoubleLinkList
src  		=main.c llist.c

$(TARGET):$(src)
	$(CC) $(src) $(CFLAGS) -o $@

clean:
	-rm -f $(TARGET)
