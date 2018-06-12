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

#pragma region Board generation

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

#pragma endregion

#pragma region Update

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

static bool findPillXY(this_p(SinglePlayerGame), int id, size_t *ox, size_t *oy) {
    size_t x, y;
    for (x = 0; x < this->board.width; x++) {
        for (y = 0; y < this->board.height; y++) {
            GameBoardElement *element = getBoardElement(&this->board, y, x);
            if (element->type == GameBoardElement_Pill && element->id == id) {
                *ox = x;
                *oy = y;
                return true;
            }
        }
    }
    return false;
}

static bool canPillMove(this_p(SinglePlayerGame), size_t x, size_t y, int id, SinglePlayerGame_Direction direction) {
    bool hasNextPill, canNextMove, hasPrevPill, canPrevMove, hasMovableDownPill, canCurrentGoDown;
	switch (direction) {
		case SinglePlayerDirection_Down:
            canCurrentGoDown = (bool) (y > 0 && (getBoardElement(&this->board, y - 1, x)->type == GameBoardElement_Empty));
            hasNextPill = (bool) (x < (this->board.width - 1) && getBoardElement(&this->board, y, x + 1)->id == id);
            canNextMove = (bool) (!hasNextPill || (y > 0 && (getBoardElement(&this->board, y - 1, x + 1)->type == GameBoardElement_Empty)));
            hasPrevPill = (bool) (x > 0 && getBoardElement(&this->board, y, x - 1)->id == id);
            canPrevMove = (bool) (!hasPrevPill || (y > 0 && (getBoardElement(&this->board, y - 1, x - 1)->type == GameBoardElement_Empty)));
            hasMovableDownPill = (bool) (y > 1 && getBoardElement(&this->board, y - 1, x)->id == id && getBoardElement(&this->board, y - 2, x)->type == GameBoardElement_Empty);
            return (bool) ((canCurrentGoDown || hasMovableDownPill) && canNextMove && canPrevMove);
        case SinglePlayerDirection_Left:
            return (bool) ((x > 0 && (getBoardElement(&this->board, y, x - 1)->type == GameBoardElement_Empty)
                        || (x > 1 && getBoardElement(&this->board, y, x - 1)->id == id && getBoardElement(&this->board, y, x - 2)->type == GameBoardElement_Empty)));
        case SinglePlayerDirection_Right:
            return (bool) ((x < this->board.width - 1 && (getBoardElement(&this->board, y, x + 1)->type == GameBoardElement_Empty)
                        || (x < this->board.width - 2 && getBoardElement(&this->board, y, x + 1)->id == id && getBoardElement(&this->board, y, x + 2)->type == GameBoardElement_Empty)));
		default:
			return false;
	}
}

static size_t columnCount(this_p(SinglePlayerGame), size_t x, size_t y, GameBoardElementColor color) {
    if (getBoardElement(&this->board, y, x)->color != color)
        return 0;
    else {
        if (y == 0)
            return 1;
        else
            return 1 + columnCount(this, x, y - 1, color);
    }
}

static void removeColumns(this_p(SinglePlayerGame), size_t x, size_t y, size_t toRemove) {
    if (toRemove > 0 && y >= 0) {
        GameBoardElement *element = getBoardElement(&this->board, y, x);
        element->id = -1;
        element->type = GameBoardElement_Empty;
        removeColumns(this, x, y - 1, toRemove - 1);
    }
}

static void removeAllPossible(this_p(SinglePlayerGame)) {
    size_t x, y;
    for (x = 0; x < this->board.width; x++) {
        for (y = 0; y < this->board.height; y++) {
            size_t toRemove = columnCount(this, x, y, getBoardElement(&this->board, y, x)->color);
            if (toRemove >= 4)
                removeColumns(this, x, y, toRemove);
        }
    }
}

static bool pillMove(this_p(SinglePlayerGame), size_t *x, size_t *y, SinglePlayerGame_Direction direction) {
    size_t pillLX, pillLY, pillRX, pillRY;
    findPillXY(this, this->currentPillId, &pillLX, &pillLY);
    pillRX = pillLX + 1; // TODO rotation

    switch (direction) {
        case SinglePlayerDirection_Down:
            moveElement(this, getBoardElement(&this->board, *y - 1, *x),
                        getBoardElement(&this->board, *y, *x));
            (*y)--;
            break;
        case SinglePlayerDirection_Left:
            // TODO: understand why the pill split

            if (pillLX > 0) {
                moveElement(this, getBoardElement(&this->board, *y, *x - 1),
                            getBoardElement(&this->board, *y, *x));
                (*x)--;
            }

            break;
        case SinglePlayerDirection_Right:
            if (pillRX < this->board.width - 1) {
                moveElement(this, getBoardElement(&this->board, *y, *x + 1),
                            getBoardElement(&this->board, *y, *x));
                (*x)++;
            }

            break;
        default:
            break;
    }
}

static bool currentPillMove(this_p(SinglePlayerGame), SinglePlayerGame_Direction direction) {
    size_t pillLX, pillLY, pillRX, pillRY;
    findPillXY(this, this->currentPillId, &pillLX, &pillLY);
    pillRX = pillLX + 1; // TODO rotation
    pillRY = pillLY;
    if (this->currentPillId >= 0
            && canPillMove(this, pillLX, pillLY, this->currentPillId, SinglePlayerDirection_Down)
            && canPillMove(this, pillRX, pillRY, this->currentPillId, SinglePlayerDirection_Down)) {
        switch (direction) {
            case SinglePlayerDirection_Left:
                pillMove(this, &pillLX, &pillLY, direction);
                pillMove(this, &pillRX, &pillRY, direction);
                break;
            case SinglePlayerDirection_Right:
                pillMove(this, &pillRX, &pillRY, direction);
                pillMove(this, &pillLX, &pillLY, direction);
                break;
            case SinglePlayerDirection_Down:
                pillMove(this, &pillLX, &pillLY, direction);
                pillMove(this, &pillRX, &pillRY, direction);
                break;
            default:
                break;
        }
        return true;
    } else {
        return false;
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
        l->id = this->currentPillId;
        r->id = this->currentPillId;
        return true;
    }
}

/* true if something was moved */
static bool applyGravity(this_p(SinglePlayerGame)) {
    size_t x, y;
    int id;
    bool moved = false;

    /* other pills */
    for (id = 0; id <= this->currentPillId; id++) {
        for (x = 0; x < this->board.width; x++) {
            for (y = 0; y < this->board.height; y++) {
                GameBoardElement *element = getBoardElement(&this->board, y, x);
                if (element->type == GameBoardElement_Pill && element->id == id
                    && canPillMove(this, x, y, element->id, SinglePlayerDirection_Down)) {
                    size_t tx = x, ty = y;
                    pillMove(this, &tx, &ty, SinglePlayerDirection_Down);
                    moved = true;
                }
            }
        }
    }
    return moved;
}


static void update(this_p(SinglePlayerGame), Engine* engine, SinglePlayerGame_Direction direction) {
    uint32_t time = VTP(engine->screen)->getCurrentTime(engine->screen);

    if (this->state == SinglePlayerState_Moving) {
        currentPillMove(this, direction);
        if (time - this->lastGravity > 2000 / ((this->speed + 1) * 1.5)) {
            if (!applyGravity(this)) {
                this->state = SinglePlayerState_Still;
                removeAllPossible(this);
                while (applyGravity((this)));
            }
            this->lastGravity = time;
        }
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
}


static void startGame(this_p(SinglePlayerGame), Engine* engine) {
    addNextPill(this);
    this->state = SinglePlayerState_Moving;
    this->lastGravity = VTP(engine->screen)->getCurrentTime(engine->screen);
}

#pragma endregion

#pragma region Constructor

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
            element->id = -1;
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

#pragma endregion
