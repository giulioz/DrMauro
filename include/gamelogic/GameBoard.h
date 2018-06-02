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
    GameBoardElement_Yellow
} GameBoardElementColor;

typedef class GameBoardElement {
    GameBoardElementType type;
    GameBoardElementColor color;
    unsigned long id;   /* same id means merged pill */
} GameBoardElement;

#endif
