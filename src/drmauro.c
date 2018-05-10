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

/* TODO: rename this function */
int clamp(int n, int max) {
	return n < max ? n : max;
}

int canPutVirus(struct cella field[RIGHE][COLONNE], int row, int col) {
	enum contenuto curCell, aboveCell, leftCell, rightCell;

	curCell = field[row][col].tipo;
	aboveCell = field[row-1][col].tipo;
	rightCell = field[row][col+1].tipo;

	if (col > 0)
		leftCell = field[row][col-1].tipo;

	else
		leftCell = field[row][col].tipo;

	if (curCell == VUOTO && aboveCell == VUOTO) {
		if (leftCell == VUOTO && rightCell == VUOTO)
			return TRUE;
	}

	return FALSE;
}

void carica_campo(struct gioco *gioco, char *percorso) {

}

void riempi_campo(struct gioco *gioco, int difficolta) {
	int randSeed = time(NULL);
	int i = 0;

	int virusNumber = 4 * (difficolta + 1);
	int upperLimit = 5;

	struct cella *curCell;
	int row, col;

	srand(randSeed);
	printf("Seed: %d\n", randSeed); /* debug */

	gioco->active_id = 0;

	for (row = 0; row < RIGHE; row++) {
		for (col = 0; col < COLONNE; col++) {
			curCell = &gioco->campo[row][col];
			curCell->tipo = VUOTO;
		}
	}

	while (i < virusNumber) {
		row = clamp(randInt(upperLimit, upperLimit+3), RIGHE-1);
		col = randInt(0, COLONNE);
		upperLimit = row;

		if (canPutVirus(gioco->campo, row, col)) {
			curCell = &gioco->campo[row][col];

			curCell->tipo = MOSTRO;
			curCell->colore = randInt(ROSSO, BLU+1);
			i++;
		}
	}
}

void step(struct gioco *gioco, enum mossa comando) {

}

enum stato vittoria(struct gioco *gioco) {
	return IN_CORSO;
}
