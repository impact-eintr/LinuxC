CFLAGS+=-pthread
LDFLAGS+=-pthread

all:mytbf

mytbf:main.c mytbf.c
	gcc $^ -g  -o  $@ $(CFLAGS) $(LDFLAGS)

clean:
	rm -rf  mytbf

.PHONY:clean ALL
