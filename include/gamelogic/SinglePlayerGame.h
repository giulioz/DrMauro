/*
 *  SinglePlayerGame.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef SINGLEPLAYERGAME_H
#define SINGLEPLAYERGAME_H

#include "Object.h"
#include "Bool.h"
#include "IntTypes.h"
#include "GameBoard.h"
#include "Vector.h"

#define SPBoardWidth 8
#define SPBoardHeight 16

typedef enum {
    SinglePlayerState_Playing,
    SinglePlayerState_EndWon,
    SinglePlayerState_EndLost,
    SinglePlayerState_Nothing
} SinglePlayerGame_State;

typedef enum {
    SinglePlayerSpeed_Low,
    SinglePlayerSpeed_Med,
    SinglePlayerSpeed_Hi
} SinglePlayerGame_Speed;

class SinglePlayerGame;

struct SinglePlayerGame_VTABLE {
    void (*update)(this_p(SinglePlayerGame), uint32_t time);
};

typedef class SinglePlayerGame {
    struct SinglePlayerGame_VTABLE *VTABLE;

    SinglePlayerGame_State state;
    int top, score;
    int level, virus;
    SinglePlayerGame_Speed speed;

    GameBoardElement boardAlloc[SPBoardWidth*SPBoardHeight];
    Vector2D board;
} SinglePlayerGame;

void SinglePlayerGame_init(this_p(SinglePlayerGame), int top, int level, int virus, SinglePlayerGame_Speed speed);

#endif
