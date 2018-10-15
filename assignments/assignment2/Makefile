CC= clang

CFLAGS= -std=c99 -Wall -pedantic

LDFLAGS= 

DEBUG= -g

all: fw

fw: fw.c hash.c
	$(CC) $(DEBUG) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(DEBUG) $(CFLAGS) -c -o $@ $^

clean:
	$(RM) fw *.o
