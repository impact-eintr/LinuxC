CFLAGS+=-Wall -g -lpthread
src = $(wildcard *.cpp)
obj = $(patsubst %.cpp,%,$(src))

ALL:$(obj)

%:%.cpp
	g++ $< -o $@ $(CFLAGS)

clean:
	-rm -rf $(obj)

.PHONY:clean ALL
