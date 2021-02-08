SHELL    = /bin/sh
CC       = clang++
CFLAGS   = -Wall -O -g
CXXFLAGS =
INCLUDE  = -I ./include
TARGET   = main
SRC      = ./main.cpp
SRCS     = ./src/ *.cpp
LDFLAGS  = -lpolymorphism
LIBPATH  = -L./lib
LIB      = ./lib/libpolymorphism.a


all:$(TARGET)

$(LIB):$(SRCS)
	cd ./src && make all

$(TARGET):$(SRC) $(LIB)
	$(CC) $(CFLAGS) $(INCLUDE) $(SRC)  $(LDFLAGS) $(LIBPATH)  -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
	cd ./src && make clean
