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

typedef enum {
    Single,
    Horizontal,
    Vertical
} WindowType;

static WindowType checkWindow(this_p(SinglePlayerGame), size_t x, size_t y) {
    GameBoardElement *thisElement = getBoardElement(&this->board, y, x);
    if (y < this->board.height - 1 && getBoardElement(&this->board, y + 1, x)->id == thisElement->id)
        return Vertical;
    else if (x < this->board.width - 1 && getBoardElement(&this->board, y, x + 1)->id == thisElement->id)
        return Horizontal;
    else
        return Single;
}

static bool moveElement(this_p(SinglePlayerGame), GameBoardElement* dest, GameBoardElement* src) {
    GameBoardElement bak = *dest;
    /*if (dest->type != GameBoardElement_Empty)
        return false;*/
    *dest = *src;
    *src = bak;
    return true;
}

static bool moveElementDirection(this_p(SinglePlayerGame), size_t x, size_t y, SinglePlayerGame_Direction direction) {
    GameBoardElement *currentElement = getBoardElement(&this->board, y, x);
    switch (direction) {
        case SinglePlayerDirection_Down:
            return moveElement(this, getBoardElement(&this->board, y - 1, x), currentElement);
        case SinglePlayerDirection_Left:
            return moveElement(this, getBoardElement(&this->board, y, x - 1), currentElement);
        case SinglePlayerDirection_Right:
            return moveElement(this, getBoardElement(&this->board, y, x + 1), currentElement);
        default:
            return false;
    }
}

static size_t countColor(this_p(SinglePlayerGame), size_t x, size_t y, int dx, int dy, GameBoardElementColor color) {
    if (x >= this->board.width || y >= this->board.height
        || getBoardElement(&this->board, y, x)->color != color
        || getBoardElement(&this->board, y, x)->type == GameBoardElement_Empty)
        return 0;
    else
        return 1 + countColor(this, x + dx, y + dy, dx, dy, color);
}

static void removeCells(this_p(SinglePlayerGame), size_t x, size_t y, int dx, int dy, size_t toRemove) {
    if (toRemove > 0) {
        GameBoardElement *element = getBoardElement(&this->board, y, x);
        if (element->type == GameBoardElement_Virus)
            this->virusCount--;
        element->id = -1;
        element->type = GameBoardElement_Empty;
        removeCells(this, x + dx, y + dy, dx, dy, toRemove - 1);
    }
}

static bool removePossible(this_p(SinglePlayerGame)) {
    size_t x, y, toRemoveVert, toRemoveHor;
    for (x = 0; x < this->board.width; x++) {
        for (y = 0; y < this->board.height; y++) {
            toRemoveVert = countColor(this, x, y, 0, 1, getBoardElement(&this->board, y, x)->color);
            if (toRemoveVert >= 4) {
                removeCells(this, x, y, 0, 1, toRemoveVert);
                return true;
            }
            toRemoveHor = countColor(this, x, y, 1, 0, getBoardElement(&this->board, y, x)->color);
            if (toRemoveHor >= 4) {
                removeCells(this, x, y, 1, 0, toRemoveHor);
                return true;
            }
        }
    }
    return false;
}

bool isBoundaryEmpty(this_p(SinglePlayerGame), size_t x, size_t y, SinglePlayerGame_Direction direction) {
    switch (direction) {
        case SinglePlayerDirection_Up:
            return (bool) (y < this->board.height - 1
                           && getBoardElement(&this->board, y + 1, x)->type == GameBoardElement_Empty);
        case SinglePlayerDirection_Down:
            return (bool) (y > 0 && getBoardElement(&this->board, y - 1, x)->type == GameBoardElement_Empty);
        case SinglePlayerDirection_Left:
            return (bool) (x > 0 && getBoardElement(&this->board, y, x - 1)->type == GameBoardElement_Empty);
        case SinglePlayerDirection_Right:
            return (bool) (x < this->board.width - 1
                           && getBoardElement(&this->board, y, x + 1)->type == GameBoardElement_Empty);
        default:
            return false;
    }
}

static bool pillMove(this_p(SinglePlayerGame), int id, SinglePlayerGame_Direction direction) {
    size_t x, y;
    for (x = 0; x < this->board.width; x++) {
        for (y = 0; y < this->board.height; y++) {
            GameBoardElement *elem = getBoardElement(&this->board, y, x);
            if (elem->id == id) {
                WindowType type = checkWindow(this, x, y);
                switch (type) {
                    case Single:
                        if (isBoundaryEmpty(this, x, y, direction))
                            return moveElementDirection(this, x, y, direction);
                        else return false;
                    case Horizontal:
                        switch (direction) {
                            case SinglePlayerDirection_Down:
                                if (isBoundaryEmpty(this, x, y, direction)
                                    && isBoundaryEmpty(this, x + 1, y, direction)) {
                                    moveElementDirection(this, x, y, direction);
                                    moveElementDirection(this, x + 1, y, direction);
                                    return true;
                                } else return false;
                            case SinglePlayerDirection_Left:
                                if (isBoundaryEmpty(this, x, y, direction)) {
                                    moveElementDirection(this, x, y, direction);
                                    moveElementDirection(this, x + 1, y, direction);
                                    return true;
                                } else return false;
                            case SinglePlayerDirection_Right:
                                if (isBoundaryEmpty(this, x + 1, y, direction)) {
                                    moveElementDirection(this, x + 1, y, direction);
                                    moveElementDirection(this, x, y, direction);
                                    return true;
                                } else return false;
                            default:
                                return false;
                        }
                    case Vertical:
                        switch (direction) {
                            case SinglePlayerDirection_Down:
                                if (isBoundaryEmpty(this, x, y, direction)) {
                                    moveElementDirection(this, x, y, direction);
                                    moveElementDirection(this, x, y + 1, direction);
                                    return true;
                                } else return false;
                            case SinglePlayerDirection_Left:
                                if (isBoundaryEmpty(this, x, y, direction)
                                    && isBoundaryEmpty(this, x, y + 1, direction)) {
                                    moveElementDirection(this, x, y, direction);
                                    moveElementDirection(this, x, y + 1, direction);
                                    return true;
                                } else return false;
                            case SinglePlayerDirection_Right:
                                if (isBoundaryEmpty(this, x, y, direction)
                                    && isBoundaryEmpty(this, x, y + 1, direction)) {
                                    moveElementDirection(this, x, y, direction);
                                    moveElementDirection(this, x, y + 1, direction);
                                    return true;
                                } else return false;
                            default:
                                return false;
                        }
                    default:
                        return false;
                }
            }
        }
    }
    return false;
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
    int id;
    bool moved = false;

    for (id = 0; id <= this->currentPillId; id++) {
        moved |= pillMove(this, id, SinglePlayerDirection_Down);
    }
    return moved;
}

static bool rotatePill(this_p(SinglePlayerGame), int id, SinglePlayerGame_Direction direction) {
    size_t x, y;
    for (x = 0; x < this->board.width; x++) {
        for (y = 0; y < this->board.height; y++) {
            GameBoardElement *elem = getBoardElement(&this->board, y, x);
            if (elem->id == id) {
                WindowType type = checkWindow(this, x, y);
                switch (type) {
                    case Horizontal:
                        if (direction == SinglePlayerDirection_RotateRight)
                            moveElement(this, getBoardElement(&this->board, y, x + 1),
                                        getBoardElement(&this->board, y, x));
                        if (isBoundaryEmpty(this, x, y, SinglePlayerDirection_Up)) {
                            moveElement(this, getBoardElement(&this->board, y + 1, x),
                                        getBoardElement(&this->board, y, x + 1));
                            return true;
                        } else if (isBoundaryEmpty(this, x, y, SinglePlayerDirection_Down)) {
                            moveElement(this, getBoardElement(&this->board, y - 1, x),
                                        getBoardElement(&this->board, y, x + 1));
                            return true;
                        } else return false;
                    case Vertical:
                        if (direction == SinglePlayerDirection_RotateLeft)
                            moveElement(this, getBoardElement(&this->board, y + 1, x),
                                        getBoardElement(&this->board, y, x));
                        if (isBoundaryEmpty(this, x, y, SinglePlayerDirection_Right)) {
                            moveElement(this, getBoardElement(&this->board, y, x + 1),
                                        getBoardElement(&this->board, y + 1, x));
                            return true;
                        } else if (isBoundaryEmpty(this, x, y, SinglePlayerDirection_Left)) {
                            moveElement(this, getBoardElement(&this->board, y, x - 1),
                                        getBoardElement(&this->board, y + 1, x));
                            return true;
                        } else return false;
                    default:
                        return false;
                }
            }
        }
    }
    return false;
}


static void update(this_p(SinglePlayerGame), Engine* engine, SinglePlayerGame_Direction direction) {
    uint32_t time = VTP(engine->screen)->getCurrentTime(engine->screen);

    /* win condition */
    if (this->virusCount <= 0) {
        this->state = SinglePlayerState_EndWon;
    }

    if (this->state == SinglePlayerState_Moving) {
        if (direction == SinglePlayerDirection_RotateRight || direction == SinglePlayerDirection_RotateLeft)
            rotatePill(this, this->currentPillId, direction);
        else
            pillMove(this, this->currentPillId, direction);

        if (time - this->lastGravity > 2000 / ((this->speed + 1) * 1.5)) {
            if (!applyGravity(this)) {
                this->state = SinglePlayerState_Still;
                removePossible(this);
            }
            this->lastGravity = time;
        }
    } else if (this->state == SinglePlayerState_Still) {
        if (time - this->lastGravity > 300 / ((this->speed + 1) * 1.5)) {
            if (!applyGravity(this)) {
                if (!removePossible(this))
                    this->state = SinglePlayerState_Ready;
            }
            this->lastGravity = time;
        }
    }

    /* create new pill */
	if (this->state == SinglePlayerState_Ready) {
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

    srand((unsigned int) time(NULL));

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
