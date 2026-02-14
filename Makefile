CC=gcc
CFLAGS=-Wall -O3 -std=c99 -s -Wno-unused-result -Wno-parentheses
LDFLAGS=-lm

all: sameshi

sameshi: main.c sameshi.h
	$(CC) $(CFLAGS) main.c -o sameshi $(LDFLAGS)

run: sameshi
	./sameshi
clean:
	rm -f sameshi