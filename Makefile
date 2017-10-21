CFLAGS = -fsanitize=address -g -Werror -Wall -Wextra -pedantic -std=c99 -D_DEFAULT_SOURCE -Isrc/include
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
	rm -r tests/link
