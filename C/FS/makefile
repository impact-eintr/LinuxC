CFLAGS		+=-Wall -g -lstdc++ -D_FILE_OFFSET_BITS=64
CC			=gcc
TARGET		=myls
OBJ			=myls.o

$(TARGET):$(OBJ)
	$(CC) $(OBJ) -o $@

$(OBJ):
	$(CC) $(src) $(CFLAGS) -c -o $@
	
clean:
	-rm -f $(OBJ)
	-rm -f $(TARGET)
