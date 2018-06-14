/*
 *  GameBoard.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "GameBoard.h"
#include "ExceptionManager.h"

static GameBoardElement* getElement(this_p(GameBoard), size_t x, size_t y) {
    if (x > this->board.width)
        ThrowWarning("Out of bounds in board width, wrapping");
    return VT(this->board)->get2D(&this->board, y, x);
}

static uint32_t randomBetween(size_t min, size_t max) {
    return (uint32_t)((rand() % (max - min)) + min);
}


/* *************************************************************** */
/* Board generation                                                */
/* *************************************************************** */

static bool isColorLegalInBoard(this_p(GameBoard), size_t x, size_t y, GameBoardElementColor target) {
    GameBoardElementColor curCell;
    GameBoardElementColor aboveCell1, aboveCell2, belowCell1, belowCell2;
    GameBoardElementColor leftCell1, leftCell2, rightCell1, rightCell2;
    bool horizontalInvalid, verticalInvalid;

    curCell = getElement(this, x, y)->color;
    aboveCell1 = getElement(this, x, y + 1)->color;
    aboveCell2 = getElement(this, x, y + 2)->color;
    belowCell1 = belowCell2 = GameBoardElement_NoColor;
    leftCell1 = leftCell2 = GameBoardElement_NoColor;
    rightCell1 = rightCell2 = GameBoardElement_NoColor;

    if (curCell != GameBoardElement_NoColor) {
        return false;
    } else {
        /* Find cells neighbourhoods if possible */
        if (y > 0)
            belowCell1 = getElement(this, x, y - 1)->color;
        if (y > 1)
            belowCell2 = getElement(this, x, y - 2)->color;
        if (x > 0)
            leftCell1 = getElement(this, x - 1, y)->color;
        if (x > 1)
            leftCell2 = getElement(this, x - 2, y)->color;
        if (x < this->board.width - 1)
            rightCell1 = getElement(this, x + 1, y)->color;
        if (x < this->board.width - 2)
            rightCell2 = getElement(this, x + 2, y)->color;

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
static bool addRandomVirus(this_p(GameBoard), uint32_t index) {
    GameBoardElement *element;
    uint32_t y, x;
    GameBoardElementColor color;

    y = randomBetween(0, this->board.height - BoardVirusUpperLimit);
    x = randomBetween(0, this->board.width);
    color = (GameBoardElementColor) (index % 3);

    if (isColorLegalInBoard(this, x, y, color)) {
        element = getElement(this, x, y);
        element->type = GameBoardElement_Virus;
        element->color = color;
        return true;
    } else {
        return false;
    }
}


/* *************************************************************** */
/* Pill Manipulation                                               */
/* *************************************************************** */

static PillWindowType checkWindow(this_p(GameBoard), size_t x, size_t y) {
    GameBoardElement *thisElement = getElement(this, x, y);
    if (y < this->board.height - 1 && getElement(this, x, y + 1)->id == thisElement->id)
        return Vertical;
    else if (x < this->board.width - 1 && getElement(this, x + 1, y)->id == thisElement->id)
        return Horizontal;
    else
        return Single;
}

static bool swapElement(this_p(GameBoard), GameBoardElement *dest, GameBoardElement *src) {
    GameBoardElement bak = *dest;
    *dest = *src;
    *src = bak;
    return true;
}

static bool swapElementDirection(this_p(GameBoard), size_t x, size_t y, PillDirection direction) {
    GameBoardElement *currentElement = getElement(this, x, y);
    switch (direction) {
        case PillDirection_Down:
            return swapElement(this, getElement(this, x, y - 1), currentElement);
        case PillDirection_Left:
            return swapElement(this, getElement(this, x - 1, y), currentElement);
        case PillDirection_Right:
            return swapElement(this, getElement(this, x + 1, y), currentElement);
        default:
            return false;
    }
}

bool isBoundaryEmpty(this_p(GameBoard), size_t x, size_t y, PillDirection direction) {
    switch (direction) {
        case PillDirection_Up:
            return (bool) (y < this->board.height - 1
                           && getElement(this, x, y + 1)->type == GameBoardElement_Empty);
        case PillDirection_Down:
            return (bool) (y > 0 && getElement(this, x, y - 1)->type == GameBoardElement_Empty);
        case PillDirection_Left:
            return (bool) (x > 0 && getElement(this, x - 1, y)->type == GameBoardElement_Empty);
        case PillDirection_Right:
            return (bool) (x < this->board.width - 1
                           && getElement(this, x + 1, y)->type == GameBoardElement_Empty);
        default:
            return false;
    }
}

static bool rotateHorizontal(this_p(GameBoard), size_t x, size_t y) {
    if (isBoundaryEmpty(this, x, y, PillDirection_Up)) {
        swapElement(this, getElement(this, x, y + 1),
                    getElement(this, x + 1, y));
        return true;
    } else if (isBoundaryEmpty(this, x, y, PillDirection_Down)) {
        swapElement(this, getElement(this, x, y - 1),
                    getElement(this, x + 1, y));
        return true;
    } else return false;
}

static bool rotateVertical(this_p(GameBoard), size_t x, size_t y) {
    if (isBoundaryEmpty(this, x, y, PillDirection_Right)) {
        swapElement(this, getElement(this, x + 1, y),
                    getElement(this, x, y + 1));
        return true;
    } else if (isBoundaryEmpty(this, x, y, PillDirection_Left)) {
        swapElement(this, getElement(this, x - 1, y),
                    getElement(this, x, y + 1));
        return true;
    } else return false;
}

static bool pillMove(this_p(GameBoard), int id, PillDirection direction) {
    size_t x, y;
    for (x = 0; x < this->board.width; x++) {
        for (y = 0; y < this->board.height; y++) {
            GameBoardElement *elem = getElement(this, x, y);
            if (elem->id == id) {
                PillWindowType type = checkWindow(this, x, y);
                switch (type) {
                    case Single:
                        if (isBoundaryEmpty(this, x, y, direction))
                            return swapElementDirection(this, x, y, direction);
                        else return false;
                    case Horizontal:
                        switch (direction) {
                            case PillDirection_Down:
                                if (isBoundaryEmpty(this, x, y, direction)
                                    && isBoundaryEmpty(this, x + 1, y, direction)) {
                                    swapElementDirection(this, x, y, direction);
                                    swapElementDirection(this, x + 1, y, direction);
                                    return true;
                                } else return false;
                            case PillDirection_Left:
                                if (isBoundaryEmpty(this, x, y, direction)) {
                                    swapElementDirection(this, x, y, direction);
                                    swapElementDirection(this, x + 1, y, direction);
                                    return true;
                                } else return false;
                            case PillDirection_Right:
                                if (isBoundaryEmpty(this, x + 1, y, direction)) {
                                    swapElementDirection(this, x + 1, y, direction);
                                    swapElementDirection(this, x, y, direction);
                                    return true;
                                } else return false;
                            case PillDirection_RotateRight:
                                swapElement(this, getElement(this, x + 1, y),
                                            getElement(this, x, y));
                                return rotateHorizontal(this, x, y);
                            case PillDirection_RotateLeft:
                                return rotateHorizontal(this, x, y);
                            default:
                                return false;
                        }
                    case Vertical:
                        switch (direction) {
                            case PillDirection_Down:
                                if (isBoundaryEmpty(this, x, y, direction)) {
                                    swapElementDirection(this, x, y, direction);
                                    swapElementDirection(this, x, y + 1, direction);
                                    return true;
                                } else return false;
                            case PillDirection_Left:
                                if (isBoundaryEmpty(this, x, y, direction)
                                    && isBoundaryEmpty(this, x, y + 1, direction)) {
                                    swapElementDirection(this, x, y, direction);
                                    swapElementDirection(this, x, y + 1, direction);
                                    return true;
                                } else return false;
                            case PillDirection_Right:
                                if (isBoundaryEmpty(this, x, y, direction)
                                    && isBoundaryEmpty(this, x, y + 1, direction)) {
                                    swapElementDirection(this, x, y, direction);
                                    swapElementDirection(this, x, y + 1, direction);
                                    return true;
                                } else return false;
                            case PillDirection_RotateLeft:
                                swapElement(this, getElement(this, x, y + 1),
                                            getElement(this, x, y));
                                return rotateVertical(this, x, y);
                            case PillDirection_RotateRight:
                                return rotateVertical(this, x, y);
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

/* true if something was moved */
static bool applyGravity(this_p(GameBoard), int endId) {
    int id;
    bool moved = false;

    for (id = 0; id <= endId; id++) {
        moved |= pillMove(this, id, PillDirection_Down);
    }
    return moved;
}


/* *************************************************************** */
/* Removal                                                         */
/* *************************************************************** */

static size_t countColor(this_p(GameBoard), size_t x, size_t y, int dx, int dy, GameBoardElementColor color) {
    if (x >= this->board.width || y >= this->board.height
        || getElement(this, x, y)->color != color
        || getElement(this, x, y)->type == GameBoardElement_Empty)
        return 0;
    else
        return 1 + countColor(this, x + dx, y + dy, dx, dy, color);
}

static int removeCells(this_p(GameBoard), size_t x, size_t y, int dx, int dy, size_t toRemove) {
    int removed = 0;
    if (toRemove > 0) {
        GameBoardElement *element = getElement(this, x, y);
        if (element->type == GameBoardElement_Virus) {
            removed++;
        }

        element->id = -1;
        element->type = GameBoardElement_Empty;
        removeCells(this, x + dx, y + dy, dx, dy, toRemove - 1);
    }
    return removed;
}

/* returns removed viruses */
static int removeFirst(this_p(GameBoard)) {
    size_t x, y, toRemoveVert, toRemoveHor;
    for (x = 0; x < this->board.width; x++) {
        for (y = 0; y < this->board.height; y++) {
            toRemoveVert = countColor(this, x, y, 0, 1, getElement(this, x, y)->color);
            if (toRemoveVert >= 4) {
                return removeCells(this, x, y, 0, 1, toRemoveVert);
            }
            toRemoveHor = countColor(this, x, y, 1, 0, getElement(this, x, y)->color);
            if (toRemoveHor >= 4) {
                return removeCells(this, x, y, 1, 0, toRemoveHor);
            }
        }
    }
    return 0;
}


/* *************************************************************** */
/* Constructor                                                     */
/* *************************************************************** */

static void initBoard(this_p(GameBoard)) {
    GameBoardElement *element;
    uint32_t x, y;

    /* Clears board */
    for (x = 0; x < this->board.width; x++) {
        for (y = 0; y < this->board.height; y++) {
            element = getElement(this, x, y);
            element->type = GameBoardElement_Empty;
            element->color = GameBoardElement_NoColor;
            element->id = -1;
        }
    }
}

static struct GameBoard_VTABLE _vtable = {
        getElement, addRandomVirus, removeFirst, pillMove, applyGravity
};

void GameBoard_init(this_p(GameBoard)) {
    VTP(this) = &_vtable;
    StackVector2D_init(&this->board, BoardWidth, BoardHeight, sizeof(GameBoardElement), this->boardAlloc);
    initBoard(this);
}
