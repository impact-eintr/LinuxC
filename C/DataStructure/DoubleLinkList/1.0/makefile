CFLAGS		+=-Wall -g -lstdc++ -D_FILE_OFFSET_BITS=64
CC			=gcc
TARGET		=DoubleLinkList
OBJ			=llist.o
src  		=llist.c

$(TARGET):$(OBJ)
	$(CC) main.c $(OBJ) -o $@

$(OBJ):$(src)
	$(CC) $(src) $(CFLAGS) -c -o $@
	
clean:
	-rm -f $(OBJ)
	-rm -f $(TARGET)
