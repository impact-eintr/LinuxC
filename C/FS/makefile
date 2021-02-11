CFLAGS		+=-Wall -g -lstdc++ -ldl -D_FILE_OFFSET_BITS=64 -D_GUN_SOURCE
CC			=gcc
TARGET		=myls
OBJ			=myls.o

$(TARGET):$(OBJ)
	$(CC) $(OBJ) -o $@

$(OBJ):$(src)
	$(CC) $(src) $(CFLAGS) -c -o $@
	
clean:
	-rm -f $(OBJ)
	-rm -f $(TARGET)
