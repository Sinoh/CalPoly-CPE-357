C = clang

CFLAGS = -std=c99 -Wall -o -pedantic 

LDFLAGS =

DEBUG = -g

HEADER = mush.h parseline.h
MUSH = mush.c parseline.c

all: $(MUSH) $(HEADER)
	$(CC) $(DEBUG) $(CFLAGS) -o mush $(MUSH)

%.o: %.c
	$(CC) $(DEBUG) $(CFLAGS) -c -o $@ $^

clean:
	$(RM) mush *.o
