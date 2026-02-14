CC=gcc
CFLAGS=-Wall -O2 -std=c99 -s -Wno-unused-result
LDFLAGS=-lm

all: tinychess

tinychess: main.c tinychess.h
	$(CC) $(CFLAGS) main.c -o tinychess $(LDFLAGS)

run: tinychess
	./tinychess

clean:
	rm -f tinychess