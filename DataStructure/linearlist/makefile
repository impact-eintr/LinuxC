CFLAGS		+=-Wall -g -D_FILE_OFFSET_BITS=64
CC				=gcc
TARGET		=linearlist
OBJ				=temp.o

$(TARGET):$(OBJ)
	$(CC) $(OBJ) -o $@

$(OBJ):$(src)
	$(CC) $(src) $(CFLAGS) -c -o $@
	
clean:
	-rm -f $(OBJ)
	-rm -f $(TARGET)
