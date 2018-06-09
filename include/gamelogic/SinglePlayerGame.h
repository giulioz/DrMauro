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
#include "Engine.h"

#define SPBoardWidth 8
#define SPBoardHeight 16
#define SPBoardVirusUpperLimit 5
#define SPMaxVirus 20

typedef enum {
    SinglePlayerState_Begin,
    SinglePlayerState_Moving,   /* pill moving */
    SinglePlayerState_Still,    /* pill still -> launch another one */
    SinglePlayerState_EndWon,
    SinglePlayerState_EndLost,
    SinglePlayerState_Nothing
} SinglePlayerGame_State;

typedef enum {
    SinglePlayerSpeed_Low,
    SinglePlayerSpeed_Med,
    SinglePlayerSpeed_Hi
} SinglePlayerGame_Speed;

typedef enum {
    SinglePlayerDirection_Down,
    SinglePlayerDirection_Left,
    SinglePlayerDirection_Right
} SinglePlayerGame_Direction;

class SinglePlayerGame;

struct SinglePlayerGame_VTABLE {
    void (*startGame)(this_p(SinglePlayerGame), Engine* engine);
    void (*update)(this_p(SinglePlayerGame), Engine* engine);
};

typedef class SinglePlayerGame {
    struct SinglePlayerGame_VTABLE *VTABLE;

    /* Current game state */
    SinglePlayerGame_State state;

    /* Stats */
    int top, score;
    int level, virusCount;
    SinglePlayerGame_Speed speed;

    /* Board */
    GameBoardElement boardAlloc[SPBoardWidth*SPBoardHeight];
    Vector2D board;

    /* Next pill in Mario hands */
    GameBoardElementColor nextPillColorL, nextPillColorR;

    /* Current pill */
    size_t pillLX, pillLY, pillRX, pillRY;
    int currentPillId;

    /* Gravity */
    uint32_t lastGravity;

} SinglePlayerGame;

void SinglePlayerGame_init(this_p(SinglePlayerGame), Engine* engine, int top, int level, int virus, SinglePlayerGame_Speed speed);

#endif
