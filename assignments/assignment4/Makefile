CC = clang

CFLAGS = -std=c00 -Wall -o -pedantic

LDFLAGS = -v

DEBUG = -g

HEADER = mytar.h createarchive.h printarchive.h extractarchive.h
MYTAR = mytar.c createarchive.c printarchive.c extractarchive.c

all: $(MYTAR) $(HEADER)
	$(CC) $(DEBUG) $(LDFLAGS) -o mytar $(MYTAR)

%.o: %.c
	$(CC) $(DEBUG) $(CFLAGS) -c -o $@ $^

clean:
	$(RM) mytar *.o *.tar

