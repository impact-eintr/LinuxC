SHELL	  = /bin/zsh
CC        = clang++
AR		  = ar
CFLAGS    = -Wall -O2 -g -W
INCLUDE   = -I ../include/
LIB_TARGET= ./libpolymorphism.a
LIBPATH   = ../lib/

SRCS = $(wildcard  ./*.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

all:$(LIB_TARGET)

$(OBJS):$(SRCS)
	$(CC) $(CFLAGS) $(INCLUDE) -c $(SRCS)

$(LIB_TARGET):$(OBJS)
	$(AR) rcs $@ $^
	-rm -f $(OBJS)
	mv $(LIB_TARGET) $(LIBPATH)

clean:
	-rm -f $(LIBPATH)*
