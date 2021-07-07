CFLAGS +=-Wall -g -O2
CC =gcc

all:vector_mult

vector_mult:main.c vector_mult.o
	$(CC) $^ -o $@ $(CFLAGS)

vector_mult.o:vector_mult.c
	$(CC) $^ -c -o $@ $(CFLAGS)

clean:
	rm -rf *.o vector_mult
