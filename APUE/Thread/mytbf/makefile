all:mytbf
CFLAGS += -Wall -g
LDFLAGS += -lpthread -lstdc++ 

mytbf:main.c mytbf.o
	gcc $^ $(CFLAGS) $(LDFLAGS) -o $@

mytbf.o:mytbf.c
	gcc $^ $(CFLAGS) $(LDFLAGS) -c -o $@
clean:
	rm -rf *.o mytbf
