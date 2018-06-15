/*
 *  SinglePlayerGame.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef SINGLEPLAYERGAME_H
#define SINGLEPLAYERGAME_H

#include "Object.h"
#include "Bool.h"
#include "CIntTypes.h"
#include "GameBoard.h"
#include "Vector.h"
#include "Engine.h"

#define SinglePlayerGame_MaxVirus 20

typedef enum {
	SinglePlayerState_FillingBoard,     /* adding viruses for animation */
    SinglePlayerState_WaitingForPill,	/* game start timeout */
    SinglePlayerState_Moving,           /* current pill going down, user control */
    SinglePlayerState_NoControl,        /* falling and elimination, no user control */
    SinglePlayerState_Ready,            /* pill arrived, add it to the top */
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
    SinglePlayerAction_Gravity,
    SinglePlayerAction_Remove,
    SinglePlayerAction_None
} SinglePlayer_Action;

class SinglePlayerGame;

struct SinglePlayerGame_VTABLE {
    bool (*addNextVirus)(this_p(SinglePlayerGame));
    void (*update)(this_p(SinglePlayerGame), Engine* engine, PillDirection direction);
};

typedef class SinglePlayerGame {
    struct SinglePlayerGame_VTABLE *VTABLE;

    /* Current game state */
    SinglePlayerGame_State state;

    /* Stats */
    size_t top, score;
    size_t level, virusCount;
    SinglePlayerGame_Speed speed;

    /* Board */
    GameBoard *board;

    /* Next pill in Mario hands */
    GameBoardElementColor nextPillColorL, nextPillColorR;

    /* Current pill */
    int currentPillId;

    /* Gravity */
    uint32_t lastGravityTime;
    SinglePlayer_Action lastAction, nextAction;
    bool lastActionResult;

	/* Score logic */
	uint32_t deletedVirusCount;
} SinglePlayerGame;

void SinglePlayerGame_init(this_p(SinglePlayerGame), Engine* engine, size_t top, size_t level, size_t virus,
                           SinglePlayerGame_Speed speed, GameBoard *board);

#endif
