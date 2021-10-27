CC = clang

CFLAGS = -std=c00 -Wall -o -pedantic

LDFLAGS =

DEBUG = -g

HENCODE = hencode.c fileio.c htable.c btree.c

HDECODE = hdecode.c fileio.c htable.c btree.c

all: $(HENCODE) $(HDECODE)

	$(CC) $(DEBUG) $(LDFLAGS) -o hencode $(HENCODE)

	$(CC) $(DEBUG) $(LDFLAGS) -o hdecode $(HDECODE)

%.o: %.c
	$(CC) $(DEBUG) $(CFLAGS) -c -o $@ $^

clean:
	$(RM) hencode *.o
	$(RM) hdecode *.o
