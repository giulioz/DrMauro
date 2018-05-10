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

int canPutVirus(struct cella bottle[RIGHE][COLONNE], int row, int col) {
	int curCell, aboveCell, belowCell, leftCell, rightCell;
	int i;

	int colorFlags[4] = {0};
	int val = 0;

	curCell = bottle[row][col].colore;
	aboveCell = bottle[row-2][col].colore;
	belowCell = -1;
	leftCell = -1;
	rightCell = -1;

	if (curCell != -1)
		return FALSE;

	if (row < RIGHE-2)
		belowCell = bottle[row+2][col].colore;

	if (col > 1)
		leftCell = bottle[row][col-2].colore;

	if (col < COLONNE-2)
		rightCell = bottle[row][col+2].colore;

	colorFlags[aboveCell+1] = 1;
	colorFlags[belowCell+1] = 1;
	colorFlags[leftCell+1] = 1;
	colorFlags[rightCell+1] = 1;

	for (i = 1; i < 4; i++)
		val += colorFlags[i];

	return val != 3 ? TRUE : FALSE;
}

int generateVirus(struct cella bottle[RIGHE][COLONNE], int virusIndex) {
	struct cella *curCell;
	int row, col, color;

	row = randInt(5, RIGHE);
	col = randInt(0, COLONNE);
	color = virusIndex % (BLU + 1); 

	while (!canPutVirus(bottle, row, col)) {
		col++;

		if (col == COLONNE) {
			col = 0;
			row++;
		}

		if (row == RIGHE) {
			return virusIndex;
		}
	}

	curCell = &bottle[row][col];
	curCell->tipo = MOSTRO;
	curCell->colore = color;

	return virusIndex + 1;
}

void carica_campo(struct gioco *gioco, char *percorso) {

}

void riempi_campo(struct gioco *gioco, int difficolta) {
	int virusNumber = 4 * (difficolta + 1);
	int randSeed = time(NULL);
	int i = 0;

	struct cella *curCell;
	int row, col;

	srand(randSeed);
	printf("Seed: %d\n", randSeed); /* debug */

	gioco->active_id = 0;

	for (row = 0; row < RIGHE; row++) {
		for (col = 0; col < COLONNE; col++) {
			curCell = &gioco->campo[row][col];
			curCell->tipo = VUOTO;
			curCell->colore = -1;
		}
	}

	while (i < virusNumber) {
		i = generateVirus(gioco->campo, i);
	}
}

void step(struct gioco *gioco, enum mossa comando) {

}

enum stato vittoria(struct gioco *gioco) {
	return IN_CORSO;
}
