/*
 *  SinglePlayerGame.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "SinglePlayerGame.h"
#include <string.h>
#include <time.h>
#include <stdio.h>

static GameBoardElement* getBoardElement(Vector2D *board, size_t row, size_t col) {
    return VTP(board)->get2D(board, row, col);
}

static uint32_t randomBetween(size_t min, size_t max) {
	return (uint32_t)((rand() % (max - min)) + min);
}

/* *************************************************************** */
/* Board generation                                                */
/* *************************************************************** */

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

/* For board init animation */
static bool addNextVirus(this_p(SinglePlayerGame), uint32_t i) {
    if (i < this->virusCount) {
        while (!tryAddVirus(&this->board, i));
		return true;
	} else {
		return false;
	}
}


/* *************************************************************** */
/* Update                                                          */
/* *************************************************************** */

static bool moveElement(this_p(SinglePlayerGame), GameBoardElement* dest, GameBoardElement* src) {
    GameBoardElement bak = *dest;
    if (dest->type != GameBoardElement_Empty)
        return false;
    *dest = *src;
    *src = bak;
    return true;
}

static bool canPillMove(this_p(SinglePlayerGame), size_t x, size_t y, SinglePlayerGame_Direction direction) {
    if (y == 0 || getBoardElement(&this->board, y-1, x)->type != GameBoardElement_Empty) {
        this->state = SinglePlayerState_Still;
        return false;
    } else {
        return true;
    }
}

static bool currentPillMove(this_p(SinglePlayerGame), SinglePlayerGame_Direction direction) {
    if (this->currentPillId >= 0 &&
        canPillMove(this, this->pillLX, this->pillLY, direction) &&
        canPillMove(this, this->pillRX, this->pillRY, direction)) {
        switch (direction) {
            case SinglePlayerDirection_Down:
                moveElement(this, getBoardElement(&this->board, this->pillLY - 1, this->pillLX),
                            getBoardElement(&this->board, this->pillLY, this->pillLX));
                moveElement(this, getBoardElement(&this->board, this->pillRY - 1, this->pillRX),
                            getBoardElement(&this->board, this->pillRY, this->pillRX));
                this->pillLY--;
                this->pillRY--;
                break;
            case SinglePlayerDirection_Left:
                moveElement(this, getBoardElement(&this->board, this->pillLY, this->pillLX - 1),
                            getBoardElement(&this->board, this->pillLY, this->pillLX));
                moveElement(this, getBoardElement(&this->board, this->pillRY, this->pillRX - 1),
                            getBoardElement(&this->board, this->pillRY, this->pillRX));
                this->pillLX--;
                this->pillRX--;
                break;
            case SinglePlayerDirection_Right:
                moveElement(this, getBoardElement(&this->board, this->pillRY, this->pillRX + 1),
                            getBoardElement(&this->board, this->pillRY, this->pillRX));
                moveElement(this, getBoardElement(&this->board, this->pillLY, this->pillLX + 1),
                            getBoardElement(&this->board, this->pillLY, this->pillLX));
                this->pillLX++;
                this->pillRX++;
                break;
            default:
                break;
        }
    }
}

static bool addNextPill(this_p(SinglePlayerGame)) {
    GameBoardElement *r, *l;
    r = getBoardElement(&this->board, this->board.height - 1, 3);
    l = getBoardElement(&this->board, this->board.height - 1, 4);

    if (r->type != GameBoardElement_Empty || l->type != GameBoardElement_Empty)
        return false; /* occupied, lose */
    else {
        this->currentPillId++;
        l->type = GameBoardElement_Pill;
        r->type = GameBoardElement_Pill;
        l->color = this->nextPillColorL;
        r->color = this->nextPillColorR;
        l->id = (unsigned long) this->currentPillId;
        r->id = (unsigned long) this->currentPillId;
        this->pillLX = 3;
        this->pillRX = 4;
        this->pillLY = this->board.height - 1;
        this->pillRY = this->board.height - 1;
    }
}


static void handlePillMove(this_p(SinglePlayerGame), SinglePlayerGame_Direction direction) {
    GameBoardElementType leftCell, rightCell;

    leftCell = rightCell = GameBoardElement_Pill;

    if (this->pillLX > 0)
        leftCell = getBoardElement(&this->board, this->pillLY, this->pillLX - 1)->type;

    if (this->pillRX < this->board.width - 1)
        rightCell = getBoardElement(&this->board, this->pillRY, this->pillRX + 1)->type;

    /* check if movement possible */
    if ((this->pillLX != 0 || direction != SinglePlayerDirection_Left) &&
        (this->pillRX != this->board.width - 1 || direction != SinglePlayerDirection_Right) &&
        (leftCell == GameBoardElement_Empty || direction != SinglePlayerDirection_Left) &&
        (rightCell == GameBoardElement_Empty || direction != SinglePlayerDirection_Right))
        currentPillMove(this, direction);
}


static void update(this_p(SinglePlayerGame), Engine* engine, SinglePlayerGame_Direction direction) {
    uint32_t time = VTP(engine->screen)->getCurrentTime(engine->screen);

    if (time - this->lastGravity > 2000 / ((this->speed + 1) * 1.5)) {
        currentPillMove(this, SinglePlayerDirection_Down);
        this->lastGravity = time;
    }

    /* create new pill */
	if (this->state == SinglePlayerState_Still) {
		if (addNextPill(this)) {
			this->nextPillColorL = (GameBoardElementColor)(randomBetween(0, 3));
			this->nextPillColorR = (GameBoardElementColor)(randomBetween(0, 3));
			this->state = SinglePlayerState_Moving;
		} else {
			this->state = SinglePlayerState_EndLost;
		}
	}

	if (this->state == SinglePlayerState_Moving) {
		handlePillMove(this, direction);
	}
}


static void startGame(this_p(SinglePlayerGame), Engine* engine) {
    addNextPill(this);
    this->state = SinglePlayerState_Moving;
    this->lastGravity = VTP(engine->screen)->getCurrentTime(engine->screen);
}


/* *************************************************************** */
/* Constructor                                                     */
/* *************************************************************** */

static struct SinglePlayerGame_VTABLE _vtable = {
        addNextVirus, startGame, update
};

static void initBoard(this_p(SinglePlayerGame)) {
    GameBoardElement *element;
    uint32_t x, y;

    srand((unsigned int) time(NULL));

    /* Clears board */
    for (x = 0; x < this->board.width; x++) {
        for (y = 0; y < this->board.height; y++) {
            element = getBoardElement(&this->board, y, x);
            element->type = GameBoardElement_Empty;
            element->color = GameBoardElement_NoColor;
            element->id = 0;
        }
    }
}

void SinglePlayerGame_init(this_p(SinglePlayerGame), Engine* engine, int top, int level, int virus, SinglePlayerGame_Speed speed) {
	VTP(this) = &_vtable;
    this->state = SinglePlayerState_FillingBoard;
    this->top = top;
    this->score = 0;
    this->level = level;
    this->virusCount = 4 * (virus + 1);
    this->speed = speed;

    /* Next pill */
	this->nextPillColorL = (GameBoardElementColor)(randomBetween(0, 3));
	this->nextPillColorR = (GameBoardElementColor)(randomBetween(0, 3));
    this->currentPillId = -1;

    /* Board */
    StackVector2D_init(&this->board, SPBoardWidth, SPBoardHeight, sizeof(GameBoardElement), this->boardAlloc);
    memset(this->boardAlloc, 0, sizeof(GameBoardElement) * SPBoardWidth * SPBoardHeight);
    initBoard(this);
}
