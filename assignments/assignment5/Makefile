C = clang

CFLAGS = -std=c00 -Wall -o -pedantic 

LDFLAGS =

DEBUG = -g

PARSELINE = parseline.c

all: $(PARSELINE)
	$(CC) $(DEBUG) $(LDFLAGS) -o parseline $(PARESELINE) -lm
%.o: %.c
	$(CC) $(DEBUG) $(CFLAGS) -c -o $@ $^

clean:
	$(RM) parseline *.o
