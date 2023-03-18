CC=clang

CFLAGS=-Wall -Wextra -Werror -pedantic -g

all: cacher

cacher: cacher.o List.o
	$(CC) $(CFLAGS) -o cacher cacher.o List.o

cacher.o: cacher.c List.h
	$(CC) $(CFLAGS) -c cacher.c

List.o: List.c List.h
	$(CC) $(CFLAGS) -c List.c

clean:
	rm -f cacher cacher.o List.o

nuke: clean
