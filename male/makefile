CC = gcc
CFLAGS = -Wall -Wextra -Wno-implicit-fallthrough -std=c17 -O2
LDFLAGS =

.PHONY: all clean

all: labirynth

labirynth: labirynth.o queue.o bitarray.o parsing.o
	$(CC) $(LDFLAGS) -o $@ $^

queue.o: queue.c queue.h types.h
	$(CC) $(CFLAGS) -c $<

labirynth.o: labirynth.c queue.o bitarray.o parsing.o
	$(CC) $(CFLAGS) -c $<

bitarray.o: bitarray.c bitarray.h types.h
	$(CC) $(CFLAGS) -c $<

parsing.o: parsing.c parsing.h types.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o labirynth