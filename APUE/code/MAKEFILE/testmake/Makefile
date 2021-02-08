OBJS=main.o tool1.o tool2.o
CC = clang
CFLAGS+= -c -Wall -g

mytool:$(OBJS)
	$(CC) $^ -o $@

%.o:%.c
	$(CC) $^ $(CFLAGS) -o $@

clean:
	$(RM) *.o mytool -r
