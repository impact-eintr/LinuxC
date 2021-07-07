CFLAGS +=-Wall -g -O2
CC =mpicc

all:vector_add

vector_add:main.c vector_add.o
	$(CC) $^ -o $@ $(CFLAGS)

vector_add.o:vector_add.c
	$(CC) $^ -c -o $@ $(CFLAGS)

clean:
	rm -rf *.o vector_add
