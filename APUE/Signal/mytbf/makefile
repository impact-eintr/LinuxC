all:mytbf
CFLAGS += -lstdc++ -Wall -g

mytbf:main.c mytbf.o
	gcc $^ -o $@

mytbf.o:mytbf.c
	gcc $^ -c -o $@
clean:
	rm -rf *.o mytbf
