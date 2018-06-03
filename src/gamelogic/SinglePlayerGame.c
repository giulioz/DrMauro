/*
 *  SinglePlayerGame.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "SinglePlayerGame.h"
#include <string.h>
#include <time.h>
#include <stdio.h>

static uint32_t randInt(size_t min, size_t max) {
	return (uint32_t) ((rand() % (max - min)) + min);
}

static GameBoardElement* getElement(Vector2D *board, size_t row, size_t col) {
	return VTP(board)->get2D(board, row, col);
}

static bool isLegal(Vector2D *board, size_t row, size_t col, GameBoardElementColor target) {
	GameBoardElementColor curCell;
    GameBoardElementColor aboveCell1, aboveCell2, belowCell1, belowCell2;
    GameBoardElementColor leftCell1, leftCell2, rightCell1, rightCell2;
	bool hor, ver;

	curCell = getElement(board, row, col)->color;
	aboveCell1 = getElement(board, row + 1, col)->color;
	aboveCell2 = getElement(board, row + 2, col)->color;
	belowCell1 = belowCell2 = GameBoardElement_NoColor;
    leftCell1 = leftCell2 = GameBoardElement_NoColor;
    rightCell1 = rightCell2 = GameBoardElement_NoColor;

	if (curCell != GameBoardElement_NoColor) {
        return false;
    } else {
        if (row > 0)
            belowCell1 = getElement(board, row - 1, col)->color;

        if (row > 1)
            belowCell2 = getElement(board, row - 2, col)->color;

        if (col > 0)
            leftCell1 = getElement(board, row, col - 1)->color;

        if (col > 1)
            leftCell2 = getElement(board, row, col - 2)->color;

        if (col < board->width - 1)
            rightCell1 = getElement(board, row, col + 1)->color;

        if (col < board->width - 2)
            rightCell2 = getElement(board, row, col + 2)->color;

        hor = (bool) ((leftCell1 == target && leftCell2 == target) ||
                      (rightCell1 == target && rightCell2 == target) ||
                      (leftCell1 == target && rightCell1 == target));

        ver = (bool) ((aboveCell1 == target && aboveCell2 == target) ||
                      (belowCell1 == target && belowCell2 == target) ||
                      (aboveCell1 == target && belowCell1 == target));

        return (bool) !(hor || ver);
    }
}

static uint32_t generateVirus(Vector2D *board, uint32_t virusIndex) {
	GameBoardElement *element;
    uint32_t row, col;
    GameBoardElementColor color;

	row = randInt(0, board->height - 5);
	col = randInt(0, board->width);
	color = (GameBoardElementColor) (virusIndex % 3);

	if (isLegal(board, row, col, color)) {
		element = getElement(board, row, col);
		element->type = GameBoardElement_Virus;
		element->color = color;

		return virusIndex + 1;
	} else {
        return virusIndex;
    }
}

static void update(this_p(SinglePlayerGame), Engine* engine) {
    uint32_t time = VTP(engine->screen)->getCurrentTime(engine->screen);
    InputState *inputState = VTP(engine->inputDevice)->getInputState(engine->inputDevice);
    this->score = inputState->downButton;

    /* funzione step */
}

static void initBoard(this_p(SinglePlayerGame), int virus) {
	GameBoardElement *element;
	uint32_t i = 0;
    uint32_t x, y;

    for (x = 0; x < this->board.width; x++) {
        for (y = 0; y < this->board.height - 4; y++) {
            element = getElement(&this->board, y, x);
            element->type = GameBoardElement_Empty;
            element->color = GameBoardElement_NoColor;
        }
    }

	while (i < virus) {
        i = generateVirus(&this->board, i);
    }
}


static struct SinglePlayerGame_VTABLE _vtable = {
        update
};

void SinglePlayerGame_init(this_p(SinglePlayerGame), int top, int level, int virus, SinglePlayerGame_Speed speed) {
	uint32_t randSeed = (uint32_t) time(NULL);

	srand(randSeed);
	printf("Seed: %d\n", randSeed); /* debug */

    VTP(this) = &_vtable;
    this->state = SinglePlayerState_Playing;
    this->top = top;
    this->score = 0;
    this->level = level;
    this->virus = 4 * (15 + 1); /* TODO */
    this->speed = speed;

    /* Board */
    StackVector2D_init(&this->board, SPBoardWidth, SPBoardHeight, sizeof(GameBoardElement), this->boardAlloc);
    memset(this->boardAlloc, 0, sizeof(GameBoardElement) * SPBoardWidth * SPBoardHeight);
    initBoard(this, this->virus);

    /* Next pill */
    this->nextPillColorL = GameBoardElement_Red;
    this->nextPillColorR = GameBoardElement_Blue;
}
