SRCS=$(wildcard ./*.cpp)
OBJS=$(patsubst %.cpp,%,$(SRCS))
CC=clang++
CFLAGS+=-Wall -g -O2

ALL:$(OBJS)

%:%.cpp
	$(CC) $(CFLAGS) $^ -o $@

clean:
	-rm -f $(OBJS)
