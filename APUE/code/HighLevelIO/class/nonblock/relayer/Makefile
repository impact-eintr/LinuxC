CC=gcc
CFLAGS+=-g -Wall -pthread
LDFLAGS+=-pthread
ALL:relayer

#relayer:main.o relayer.o
#	$(CC) $^ $(CFLAGS) -o $@

#%.o:%.c
#	$(CC) $^ $(CFLAGS) -o $@
#
relayer:main.c relayer.c
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@

clean:
	$(RM) *.o relayer -r
