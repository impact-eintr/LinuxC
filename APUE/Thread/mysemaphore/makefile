all:mysem
CFLAGS += -Wall -g
LDFLAGS += -lpthread -lstdc++ 

mysem:main.c mysem.o
	gcc $^ $(CFLAGS) $(LDFLAGS) -o $@

mysem.o:mysem.c
	gcc $^ $(CFLAGS) $(LDFLAGS) -c -o $@
clean:
	rm -rf *.o mysem
