CC       = gcc
CFLAGS   = -Wall -O -g
CXXFLAGS =
INCLUDE  = -I ./comm/inc -I ./staticlib/inc
TARGET   = atexit
LIBVAR   = -lmytest   链接 libmytest.a
LIBPATH  = -L./staticlib/lib
#search paths for errorhandler.c
vpath %.c ./comm
#下行是为依赖项 apue.h 准备的，比如 [errorhandler.o:errorhandler.c apue.h] 里的 apue.h
vpath %.h ./comm/inc

OBJS     = errorhandler.o atexit.o
#下行的 apue.h，可以不必写出来
errorhandler.o:errorhandler.c apue.h
	$(CC) $(CFLAGS) $(INCLUDE) -c $^
	atexit.o:atexit.c apue.h
	$(CC) $(CFLAGS) $(INCLUDE) -c $^

all:$(OBJS) $(LIB)
	cd ./staticlib && make all
	$(CC) $(CFLAGS) $(INCLUDE) -o $(TARGET) $(OBJS) $(LIBVAR) $(LIBPATH)

clean:
	rm -f *.o
	rm -f $(TARGET)
	cd ./staticlib && make clean
