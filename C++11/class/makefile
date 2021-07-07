CFLAGS	+=-Wall -g -lstdc++ -O2
bin = ./bin/
src = $(wildcard *.cpp)
obj = $(patsubst %.cpp,%,$(src))

ALL:$(obj)

%:%.cpp
	gcc $< -o $(bin)$@ $(CFLAGS)

clean:
	-rm -i $(bin)*

.PHONY:clean ALL
