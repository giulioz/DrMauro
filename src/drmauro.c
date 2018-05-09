#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "drmauro.h"

#define TRUE 1
#define FALSE 0


/* Generate a random number between min and max (excluded) */
int randInt(int min, int max) {
	return (rand() % (max - min)) + min;
}

void carica_campo(struct gioco *gioco, char *percorso) {

}

void riempi_campo(struct gioco *gioco, int difficolta) {
	int virusNumber = 4 * (difficolta + 1);

	struct cella *curCell;
	int row, col, i;

	srand(time(NULL));

	gioco->punti = 0;
	gioco->active_id = 0;

	for (row = 0; row < RIGHE; row++) {
		for (col = 0; col < COLONNE; col++) {
			curCell = &gioco->campo[row][col];
			curCell->tipo = VUOTO;
		}
	}

	for (i = 0; i < virusNumber; i++) {
		row = randInt(5, RIGHE);
		col = randInt(0, COLONNE);

		/* TODO: add missing check */
		curCell = &gioco->campo[row][col];
		curCell->tipo = MOSTRO;
		curCell->colore = randInt(ROSSO, BLU+1);
	}
}

void step(struct gioco *gioco, enum mossa comando) {

}

enum stato vittoria(struct gioco *gioco) {
	return IN_CORSO;
}
