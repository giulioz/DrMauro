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

int isLegal(struct cella bottle[RIGHE][COLONNE], int row, int col, enum colore target) {
	int curCell, aboveCell1, aboveCell2, belowCell1, belowCell2, leftCell1, leftCell2, rightCell1, rightCell2;
	int hor, ver;

	curCell = bottle[row][col].colore;
	aboveCell1 = bottle[row-1][col].colore;
	aboveCell2 = bottle[row-2][col].colore;
	belowCell1 = -1;
	belowCell2 = -1;
	leftCell1 = -1;
	leftCell2 = -1;
	rightCell1 = -1;
	rightCell2 = -1;

	if (curCell != -1)
		return FALSE;

	if (row < RIGHE-1)
		belowCell1 = bottle[row+1][col].colore;

	if (row < RIGHE-2)
		belowCell2 = bottle[row+2][col].colore;

	if (col > 0)
		leftCell1 = bottle[row][col-1].colore;

	if (col > 1)
		leftCell2 = bottle[row][col-2].colore;

	if (col < COLONNE-1)
		rightCell1 = bottle[row][col+1].colore;

	if (col < COLONNE-2)
		rightCell2 = bottle[row][col+2].colore;

	hor = (leftCell1 == target && leftCell2 == target)   ||
		  (rightCell1 == target && rightCell2 == target) ||
		  (leftCell1 == target && rightCell1 == target);

	ver = (aboveCell1 == target && aboveCell2 == target) ||
		  (belowCell1 == target && belowCell2 == target) ||
		  (aboveCell1 == target && belowCell1 == target);

	return hor + ver == 0;
}

int generateVirus(struct cella bottle[RIGHE][COLONNE], int virusIndex) {
	struct cella *curCell;
	int row, col, color;

	row = randInt(5, RIGHE);
	col = randInt(0, COLONNE);
	color = virusIndex % (BLU + 1); 

	if (isLegal(bottle, row, col, color)) {
		curCell = &bottle[row][col];
		curCell->tipo = MOSTRO;
		curCell->colore = color;

		return virusIndex + 1;
	}

	return virusIndex;
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

	if (difficolta < 0)
		difficolta = 0;

	else if (difficolta > 15)
		difficolta = 15;

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
