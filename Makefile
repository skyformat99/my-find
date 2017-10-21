CFLAGS = -fsanitize=address -Werror -Wall -Wextra -pedantic -std=c99 -D_DEFAULT_SOURCE -Isrc/include
CC = gcc

SRC = $(wildcard *.c) $(wildcard */*.c)

all: myfind

myfind:
	$(CC) $(SRC) -o myfind $(CFLAGS)

test:	myfind
	python3 tests/test_suite.py

doc:
	@doxygen Doxyfile &> /dev/null
	@echo Doc generated in doc folder ..

clean:
	rm myfind
	rm -r doc tests/architecture tests/link
