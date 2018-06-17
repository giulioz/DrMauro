/*
 *  CommonGameUtils.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef COMMONGAMEUTILS_H
#define COMMONGAMEUTILS_H

#include "Object.h"
#include "GameState.h"
#include "Graphics.h"
#include "Engine.h"
#include "Assets.h"
#include "Bool.h"
#include "Sprite.h"
#include "SinglePlayerGame.h"
#include "Vector.h"
#include "CIntTypes.h"
#include "Timeline.h"
#include "GameBoard.h"

typedef enum {
    Normal,
    Left,
    Right,
    Top,
    Bottom
} PillType;

bool isVirusPresent(GameBoard *board, GameBoardElementColor virusColor);
void drawVirus(Screen* screen, uint32_t row, uint32_t col, GameBoardElementColor color);
Texture* getPillTexture(GameBoardElementColor color, PillType type);
void drawPill(Screen* screen, uint32_t row, uint32_t col, GameBoardElementColor color, PillType type);
PillType checkPillNeighborhoods(Vector2D *board, uint32_t x, uint32_t y);

#endif
