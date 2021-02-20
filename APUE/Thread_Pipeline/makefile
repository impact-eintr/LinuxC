all:mypipe
CFLAGS += -Wall -g
LDFLAGS += -lpthread -lstdc++ 

mypipe:main.c mypipe.o
	gcc $^ $(CFLAGS) $(LDFLAGS) -o $@

mypipe.o:mypipe.c
	gcc $^ $(CFLAGS) $(LDFLAGS) -c -o $@
clean:
	rm -rf *.o mypipe
