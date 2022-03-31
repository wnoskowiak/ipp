CC = gcc
CFLAGS = -Wall -Wextra -Wno-implicit-fallthrough -std=c17 -O2
LDFLAGS =

.PHONY: all clean

all: labirynth

labirynth: queue.o labirynth.o bitarray.o
	$(CC) $(LDFLAGS) -o $@ $^

queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c $<

labirynth.o: labirynth.c queue.o
	$(CC) $(CFLAGS) -c $<

bitarray.o: bitarray.c bitarray.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o labirynth