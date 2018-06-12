/*
 *  GameBoard.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "Object.h"
#include "Bool.h"
#include "IntTypes.h"

typedef enum {
    GameBoardElement_Empty,
    GameBoardElement_Virus,
    GameBoardElement_Pill
} GameBoardElementType;

typedef enum {
    GameBoardElement_Red,
    GameBoardElement_Blue,
    GameBoardElement_Yellow,
    GameBoardElement_NoColor
} GameBoardElementColor;

typedef class GameBoardElement {
    GameBoardElementType type;
    GameBoardElementColor color;
    int id;   /* same id means merged pill, ignored for virus */
} GameBoardElement;

#endif
