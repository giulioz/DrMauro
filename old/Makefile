.PHONY: clean

CFLAGS=-std=gnu89 -pedantic -Wall -g
LDFLAGS=-lSDL2 -lm

all: drmauro

game.o: game.c game.h
	gcc $(CFLAGS) -c -o $@ $<

drmauro.o: drmauro.c drmauro.h
	gcc $(CFLAGS) -c -o $@ $<

drmauro_main.o: drmauro_main.c drmauro.h game.h
	gcc $(CFLAGS) -c -o $@ $<

drmauro: drmauro_main.o drmauro.o game.o
	gcc -o $@ $^ $(LDFLAGS)

clean:
	rm drmauro
	rm *.o
