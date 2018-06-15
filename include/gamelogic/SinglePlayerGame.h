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

#define SPMaxVirus 20

typedef enum {
	SinglePlayerState_FillingBoard,
    SinglePlayerState_Begin,	/* still to begin */
    SinglePlayerState_Moving,   /* pill moving */
    SinglePlayerState_Still,    /* pill still, waiting for falling */
    SinglePlayerState_Ready,    /* next pill */
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
    bool (*addNextVirus)(this_p(SinglePlayerGame), uint32_t i);
    void (*startGame)(this_p(SinglePlayerGame), Engine* engine);
    void (*update)(this_p(SinglePlayerGame), Engine* engine, PillDirection direction);
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
    GameBoard *board;

    /* Next pill in Mario hands */
    GameBoardElementColor nextPillColorL, nextPillColorR;

    /* Current pill */
    int currentPillId;

    /* Gravity */
    uint32_t lastGravity;

	/* Score logic */
	uint32_t deletedVirusCount;
} SinglePlayerGame;

void SinglePlayerGame_init(this_p(SinglePlayerGame), Engine* engine, int top, int level, int virus,
                           SinglePlayerGame_Speed speed, GameBoard *board);

#endif
