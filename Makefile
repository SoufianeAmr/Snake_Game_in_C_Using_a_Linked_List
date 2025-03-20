CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -D_XOPEN_SOURCE=700

all: snake

snake: main.o game.o
	$(CC) $(CFLAGS) -o snake main.o game.o

main.o: main.c game.h
	$(CC) $(CFLAGS) -c main.c

game.o: game.c game.h
	$(CC) $(CFLAGS) -c game.c

clean:
	rm -f *.o snake

