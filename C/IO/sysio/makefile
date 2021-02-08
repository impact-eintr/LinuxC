#CFLAGS		+=-Wall -g -lstdc++ -D_FILE_OFFSET_BITS=64
CFLAGS		+=-Wall -g -lstdc++
CC			=gcc
TARGET		=sysio
OBJ			=sysio.o

$(TARGET):$(OBJ)
	$(CC) $(OBJ) -o $@

$(OBJ):
	$(CC) $(src) $(CFLAGS) -c -o $@
	
clean:
	-rm -f $(OBJ)
	-rm -f $(TARGET)
