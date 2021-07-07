CFLAGS += -Wall -g -lm -O2
CC = mpicc

all:trap

trap:main.c trap.o
	$(CC) $^ -o $@ $(CFLAGS)

trap.o:trap.c
	$(CC) $^ -c -o $@ $(CFLAGS)

clean:
	rm -rf *.o trap
