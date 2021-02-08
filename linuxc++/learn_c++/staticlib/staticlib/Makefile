CC        = gcc
CFLAGS    = -Wall -O -g
CXXFLAGS  =
INCLUDE   = -I ./inc -I ../comm/inc
TARGET    = libmytest.a
LIBPATH   = ./lib/

vpath %.h ./inc

OBJS      = staticlib_add.o staticlib_mul.o
SRCS      = staticlib_add.c staticlib_mul.c

$(OBJS):$(SRCS)
	$(CC) $(CFLAGS) $(INCLUDE) -c $^

all:$(OBJS)
	ar rcs $(TARGET) $^
	mv $(TARGET) $(LIBPATH)

clean:
	rm -f *.o
	rm -f $(LIBPATH)*
