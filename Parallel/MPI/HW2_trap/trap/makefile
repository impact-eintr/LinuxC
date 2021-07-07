CFLAGS += -Wall -lm -g -O2

all:trap

trap:main.c trap.o
	gcc $^ -o $@ $(CFLAGS)

trap.o:trap.c
	gcc $^ -c -o $@ $(CFLAGS)

clean:
	rm -rf *.o trap
