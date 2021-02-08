CFLAGS+=-pthread
LDFLAGS+=-pthread

all:mysem

mysem:main.c mysem.c
	gcc $^ -g  -o  $@ $(CFLAGS) $(LDFLAGS)

clean:
	rm -rf  mysem

.PHONY:clean ALL
