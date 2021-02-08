src = $(wildcard *.cpp)
obj = $(patsubst %.cpp,%,$(src))

ALL:$(obj)

%:%.cpp
	g++ $< -o $@ -g -Wall

clean:
	-rm -rf $(obj)

.PHONY:clean ALL
