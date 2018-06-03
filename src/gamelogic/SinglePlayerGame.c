/*
 *  SinglePlayerGame.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "SinglePlayerGame.h"
#include <string.h>
#include <time.h>
#include <stdio.h>

int randInt(int min, int max) {
	return (rand() % (max - min)) + min;
}

int getColor(Vector2D board, int row, int col) {
	GameBoardElement *element = VT(board)->get2D(&board, row, col);
	return element->color;
}

int isLegal(Vector2D board, int row, int col, GameBoardElementColor target) {
	int curCell, aboveCell1, aboveCell2, belowCell1, belowCell2, leftCell1, leftCell2, rightCell1, rightCell2;
	int hor, ver;

	curCell = getColor(board, row, col);
	aboveCell1 = getColor(board, row+1, col);
	aboveCell2 = getColor(board, row+2, col);
	belowCell1 = -1;
	belowCell2 = -1;
	leftCell1 = -1;
	leftCell2 = -1;
	rightCell1 = -1;
	rightCell2 = -1;

	if (curCell != -1)
		return 0;

	if (row > 0)
		belowCell1 = getColor(board, row-1, col);

	if (row > 1)
		belowCell2 = getColor(board, row-2, col);

	if (col > 0)
		leftCell1 = getColor(board, row, col-1);

	if (col > 1)
		leftCell2 = getColor(board, row, col-2);

	if (col < board.width-1)
		rightCell1 = getColor(board, row, col+1);

	if (col < board.width-2)
		rightCell2 = getColor(board, row, col+2);

	hor = (leftCell1 == target && leftCell2 == target)   ||
		  (rightCell1 == target && rightCell2 == target) ||
		  (leftCell1 == target && rightCell1 == target);

	ver = (aboveCell1 == target && aboveCell2 == target) ||
		  (belowCell1 == target && belowCell2 == target) ||
		  (aboveCell1 == target && belowCell1 == target);

	return hor + ver == 0;
}

int generateVirus(Vector2D board, int virusIndex) {
	GameBoardElement *element;
    uint32_t row, col, color;

	row = randInt(0, board.height-5);
	col = randInt(0, board.width);
	color = virusIndex % 3;

	if (isLegal(board, row, col, color)) {
		element = VT(board)->get2D(&board, row, col);
		element->type = GameBoardElement_Virus;
		element->color = color;

		return virusIndex + 1;
	}

	return virusIndex;
}

static void update(this_p(SinglePlayerGame), Engine* engine) {
    uint32_t time = VTP(engine->screen)->getCurrentTime(engine->screen);
    InputState *inputState = VTP(engine->inputDevice)->getInputState(engine->inputDevice);
    this->score = inputState->downButton;

    /* funzione step */
}

static void initBoard(this_p(SinglePlayerGame), int virus) {
	GameBoardElement *element;
	//uint32_t virusNumber = 4 * (20 + 1);
	uint32_t i = 0;

    uint32_t x, y;

    for (x = 0; x < this->board.width; x++) {
        for (y = 0; y < this->board.height - 4; y++) {
            element = VT(this->board)->get2D(&this->board, y, x);
            element->type = GameBoardElement_Empty;
            element->color = -1;
        }
    }

	while (i < virus)
		i = generateVirus(this->board, i);
}


static struct SinglePlayerGame_VTABLE _vtable = {
        update
};

void SinglePlayerGame_init(this_p(SinglePlayerGame), int top, int level, int virus, SinglePlayerGame_Speed speed) {
	uint32_t randSeed = time(NULL);

	srand(randSeed);
	printf("Seed: %d\n", randSeed); /* debug */

    VTP(this) = &_vtable;
    this->state = SinglePlayerState_Playing;
    this->top = top;
    this->score = 0;
    this->level = level;
    this->virus = 4 * (15 + 1);
    this->speed = speed;

    /* Board */
    StackVector2D_init(&this->board, SPBoardWidth, SPBoardHeight, sizeof(GameBoardElement), this->boardAlloc);
    memset(this->boardAlloc, 0, sizeof(GameBoardElement) * SPBoardWidth * SPBoardHeight);
    initBoard(this, this->virus);
}
