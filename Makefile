CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99
LDLIBS = -lm

tinychess: main.o

run: tinychess
	./tinychess

clean:
	rm -f tinychess *.o