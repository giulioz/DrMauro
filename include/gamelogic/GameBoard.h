/*
 *  GameBoard.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  A DrMauro generic game board
 */

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

/* Hard-coded board sizes */
#define BoardWidth 8
#define BoardHeight 16
#define BoardVirusUpperLimit 5

#include "Object.h"
#include "Bool.h"
#include "Vector.h"
#include "CIntTypes.h"
#include "Random.h"

typedef enum {
    GameBoardElement_Empty,
    GameBoardElement_Virus,
    GameBoardElement_Pill
} GameBoardElementType;

typedef enum {
    GameBoardElement_Blue,
    GameBoardElement_Yellow,
    GameBoardElement_Red,
    GameBoardElement_NoColor
} GameBoardElementColor;

typedef enum {
    PillDirection_Up,
    PillDirection_Down,
    PillDirection_Left,
    PillDirection_Right,
    PillDirection_RotateLeft,
    PillDirection_RotateRight,
    PillDirection_Nothing
} PillDirection;

typedef enum {
    Single,
    Horizontal,
    Vertical
} PillWindowType;

typedef class GameBoardElement {
    GameBoardElementType type;
    GameBoardElementColor color;
    int id;   /* same id means merged pill, ignored for virus */
} GameBoardElement;

class GameBoard;

struct GameBoard_VTABLE {
    /* gets the pointer to an element of the board */
    GameBoardElement* (*getElement)(this_p(GameBoard), size_t x, size_t y);

    /* adds a new random virus and returns the pointer to it */
    GameBoardElement* (*addRandomVirus)(this_p(GameBoard));

    /* try remove rows of 4 or more elements */
    bool (*removeFirst)(this_p(GameBoard), int *removedViruses, GameBoardElementColor *removedColor);

    /* move a pill with a given direction */
    bool (*pillMove)(this_p(GameBoard), int id, PillDirection direction);

    /* move pills down */
    bool (*applyGravity)(this_p(GameBoard), int endId);
};

typedef class GameBoard {
    struct GameBoard_VTABLE *VTABLE;

    GameBoardElement boardAlloc[BoardWidth*BoardHeight];
    Vector2D board;
    size_t virusCount;

    /* for multiplayer mirroring */
    GameBoardElement* lastAddedVirus;
    uint32_t lastAddedX, lastAddedY;
    Random *random;
} GameBoard;

void GameBoard_init(this_p(GameBoard), Random *random);

#endif
