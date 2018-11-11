CC = clang

CFLAGS = -std=c00 -Wall -o -pedantic

LDFLAGS =

DEBUG = -g

all: htable

htable: htable.c btree.c
	$(CC) $(DEBUG) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(DEBUG) $(CFLAGS) -c -o $@ $^

clean:
	$(RM) htable *.o
