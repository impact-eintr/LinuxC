SHELL = /bin/sh
CC = g++
AR = ar

CFLAG = -O2 -W -Wall

INCLUDE_PATH = -I.
LIB_TARGET = ./libmath.a

SRCS = $(wildcard ./*.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

all : $(LIB_TARGET)


$(OBJS):$(SRCS)
	$(CC) $(CFLAG) $(INCLUDE_PATH) -c $(SRCS)


$(LIB_TARGET) :  $(OBJS)
	$(AR) rcs $@ $^
	rm -f $(OBJS)

clean:
	rm -f $(PLUS_OBJ)
	rm -f $(LIB_TARGET)
