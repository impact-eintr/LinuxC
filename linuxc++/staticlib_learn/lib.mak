SHELL = /bin/sh
CC = g++
AR = ar

CFLAG = -O2 -W -Wall

INCLUDE_PATH = -I.
LIB_TARGET = ./libmath.a
PLUS_OBJ = plus.o
PLUS_SRC = ./plus.cpp

MIN_OBJ = minus.o
MIN_SRC = ./minus.cpp

all : $(LIB_TARGET)


$(PLUS_OBJ): $(PLUS_SRC)
	$(CC) $(CFLAG) $(INCLUDE_PATH) -c $(PLUS_SRC)

$(MIN_OBJ): $(MIN_SRC)
	$(CC) $(CFLAG) $(INCLUDE_PATH) -c $(MIN_SRC)

$(LIB_TARGET) : $(PLUS_OBJ)  $(MIN_OBJ)
	$(AR) cq $@ $^
	rm -f $(PLUS_OBJ)
	rm -f $(MIN_OBJ)

clean:
	rm -f $(PLUS_OBJ)
	gm -f $(LIB_TARGET)
