CC = clang

CFLAGS = -std=c00 -Wall -o -pedantic

LDFLAGS =

DEBUG = -g

MYPWD = mypwd.c

all: $(MYPWD)

	$(CC) $(DEBUG) $(LDFLAGS) -o mypwd $(MYPWD)

%.o: %.c
	$(CC) $(DEBUG) $(CFLAGS) -c -o $@ $^

clean:
	$(RM) mypwd *.o

