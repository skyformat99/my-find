CFLAGS = -g -Werror -Wall -Wextra -pedantic -std=c99 -D_GNU_SOURCE -Isrc/header
CC = gcc

SRC = $(wildcard *.c) $(wildcard */*.c)

all: myfind

myfind:
	$(CC) $(SRC) -o myfind $(CFLAGS)

test:	myfind
	python3 tests/test_suite.py

clean:
	rm myfind
	rm -r tests/architecture
