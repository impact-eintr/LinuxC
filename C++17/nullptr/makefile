CFLAGS		+=-Wall -g -std=c++2a
CC = clang++
bin = ./bin/
src = $(wildcard *.cpp)
obj = $(patsubst %.cpp,%,$(src))

ALL:$(obj)

%:%.cpp
	$(CC) $< -o $(bin)$@ $(CFLAGS)

clean:
	-rm -i $(bin)*

.PHONY:clean ALL
