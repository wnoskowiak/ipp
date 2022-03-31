CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS =

.PHONY: all clean

all: labirynth

labirynth: queue.o labirynth.o 
	$(CC) $(LDFLAGS) -o $@ $^

queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c $<

labirynth.o: labirynth.c queue.o
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o labirynth