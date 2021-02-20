all:anytimer
CFLAGS += -lstdc++ -Wall -g

anytimer:main.c anytimer.o
	gcc $^ -o $@

anytimer.o:anytimer.c
	gcc $^ -c -o $@
clean:
	rm -rf *.o anytimer
