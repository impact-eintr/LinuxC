src = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp,%.o,$(src))


CFLAGS+=-Wall -g
CC=clang++

all:polymorphism

polymorphism:$(OBJ)
	$(CC) $^ -o $@

%.o:%.cpp
	$(CC) -c $^ -o $@ $(CFLAGS)

clean:
	-rm -rf polymorphism $(OBJ)

.PHONY:clean ALL
