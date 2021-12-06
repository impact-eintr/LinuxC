CC     = clang++
CFLAGS += -Wall -g

main:main.o
	$(CC) $^ $(CFLAGS) -o $@

main.o:main.cpp
	$(CC) $^ $(CFLAGS) -c -o $@

clean:
	$(RM) -r main main.o
