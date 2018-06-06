/*
 *  SinglePlayerGame.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "SinglePlayerGame.h"
#include <string.h>
#include <time.h>
#include <stdio.h>

/* *************************************************************** */
/* Board generation                                                */
/* *************************************************************** */

static uint32_t randomBetween(size_t min, size_t max) {
	return (uint32_t) ((rand() % (max - min)) + min);
}

static GameBoardElement* getBoardElement(Vector2D *board, size_t row, size_t col) {
	return VTP(board)->get2D(board, row, col);
}

static bool isColorLegalInBoard(Vector2D *board, size_t row, size_t col, GameBoardElementColor target) {
	GameBoardElementColor curCell;
    GameBoardElementColor aboveCell1, aboveCell2, belowCell1, belowCell2;
    GameBoardElementColor leftCell1, leftCell2, rightCell1, rightCell2;
	bool horizontalInvalid, verticalInvalid;

	curCell = getBoardElement(board, row, col)->color;
	aboveCell1 = getBoardElement(board, row + 1, col)->color;
	aboveCell2 = getBoardElement(board, row + 2, col)->color;
	belowCell1 = belowCell2 = GameBoardElement_NoColor;
    leftCell1 = leftCell2 = GameBoardElement_NoColor;
    rightCell1 = rightCell2 = GameBoardElement_NoColor;

	if (curCell != GameBoardElement_NoColor) {
        return false;
    } else {
        /* Find cells neighbourhoods if possible */
        if (row > 0)
            belowCell1 = getBoardElement(board, row - 1, col)->color;
        if (row > 1)
            belowCell2 = getBoardElement(board, row - 2, col)->color;
        if (col > 0)
            leftCell1 = getBoardElement(board, row, col - 1)->color;
        if (col > 1)
            leftCell2 = getBoardElement(board, row, col - 2)->color;
        if (col < board->width - 1)
            rightCell1 = getBoardElement(board, row, col + 1)->color;
        if (col < board->width - 2)
            rightCell2 = getBoardElement(board, row, col + 2)->color;

        horizontalInvalid = (bool) ((leftCell1 == target &&  leftCell2 == target) ||
                                   (rightCell1 == target && rightCell2 == target) ||
                                    (leftCell1 == target && rightCell1 == target));
        verticalInvalid =  (bool) ((aboveCell1 == target && aboveCell2 == target) ||
                                   (belowCell1 == target && belowCell2 == target) ||
                                   (aboveCell1 == target && belowCell1 == target));

        return (bool) !(horizontalInvalid || verticalInvalid);
    }
}

/* Returns true if virus was added */
static bool tryAddVirus(Vector2D *board, uint32_t virusIndex) {
	GameBoardElement *element;
    uint32_t row, col;
    GameBoardElementColor color;

	row = randomBetween(0, board->height - SPBoardVirusUpperLimit);
	col = randomBetween(0, board->width);
	color = (GameBoardElementColor) (virusIndex % 3);

	if (isColorLegalInBoard(board, row, col, color)) {
		element = getBoardElement(board, row, col);
		element->type = GameBoardElement_Virus;
		element->color = color;
		return true;
	} else {
        return false;
    }
}

static void initBoard(this_p(SinglePlayerGame), int virus) {
	GameBoardElement *element;
	uint32_t i = 0;
    uint32_t x, y;
    uint32_t randSeed;

    /* Random Seed */
    randSeed = (uint32_t) time(NULL);
    srand(randSeed);
    printf("Seed: %d\n", randSeed); /* debug */

    /* Clears board */
    for (x = 0; x < this->board.width; x++) {
        for (y = 0; y < this->board.height; y++) {
            element = getBoardElement(&this->board, y, x);
            element->type = GameBoardElement_Empty;
            element->color = GameBoardElement_NoColor;
        }
    }

	while (i < virus) {
        if (tryAddVirus(&this->board, i)) {
            i++;
        }
    }
}

/* *************************************************************** */
/* Update                                                          */
/* *************************************************************** */

static void update(this_p(SinglePlayerGame), Engine* engine) {
    uint32_t time = VTP(engine->screen)->getCurrentTime(engine->screen);
    InputState *inputState = VTP(engine->inputDevice1)->getInputState(engine->inputDevice1);
    this->score = inputState->downButton;

    /* funzione step */
}


/* *************************************************************** */
/* Constructor                                                     */
/* *************************************************************** */

static struct SinglePlayerGame_VTABLE _vtable = {
        update
};

void SinglePlayerGame_init(this_p(SinglePlayerGame), int top, int level, int virus, SinglePlayerGame_Speed speed) {
	VTP(this) = &_vtable;
    this->state = SinglePlayerState_Begin;
    this->top = top;
    this->score = 0;
    this->level = level;
    this->virus = virus; /*4 * (15 + 1); /* TODO */
    this->speed = speed;

    /* Board */
    StackVector2D_init(&this->board, SPBoardWidth, SPBoardHeight, sizeof(GameBoardElement), this->boardAlloc);
    memset(this->boardAlloc, 0, sizeof(GameBoardElement) * SPBoardWidth * SPBoardHeight);
    initBoard(this, this->virus);

    /* Next pill */
    this->nextPillColorL = GameBoardElement_Red;
    this->nextPillColorR = GameBoardElement_Blue;
}
