CFLAGS		+=-Wall -g -lstdc++
CC			=gcc
TARGET		=stdio
OBJ			=stdio.o

$(TARGET):$(OBJ)
	$(CC) $(OBJ) -o $@

$(OBJ):
	$(CC) $(src) $(CFLAGS) -c -o $@
	
clean:
	-rm -f $(OBJ)
	-rm -f $(TARGET)
