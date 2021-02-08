SHELL = /bin/sh
CC = g++

INCLUDE_PATH = -I.
TEST_TARGET = ./test
TEST_SRC = ./lib_test.cpp
LDFLAGS := ./libmath.a
all :
	$(CC) -g $(TEST_SRC) $(LDFLAGS) -o $(TEST_TARGET) $(INCLUDE_PATH)

clean:
	rm -f $(TEST_TARGET)
