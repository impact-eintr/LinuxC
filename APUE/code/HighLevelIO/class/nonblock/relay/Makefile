CFLAGS+=-pthread
LDFLAGS+=-pthread

all:relay

relay:main.c relay.c
	gcc $^ -g  -o  $@ $(CFLAGS) $(LDFLAGS)

clean:
	rm -rf  relay

.PHONY:clean ALL
