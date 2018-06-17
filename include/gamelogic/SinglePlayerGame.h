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
#include "GameSpeedProvider.h"
#include "SinglePlayerGame_State.h"

#define SinglePlayerGame_MaxVirus 20

typedef enum {
    SinglePlayerAction_Gravity,
    SinglePlayerAction_Remove
} SinglePlayer_Action;

class SinglePlayerGame;

struct SinglePlayerGame_VTABLE {
    void (*update)(this_p(SinglePlayerGame), Engine* engine, PillDirection direction);
};

typedef class SinglePlayerGame {
    struct SinglePlayerGame_VTABLE *VTABLE;

    /* Current game state */
    SinglePlayerGame_State state;

    /* Stats */
    size_t top, score;
    size_t level, virusCount;

    /* Board */
    GameBoard *board;

    /* Next pill in Mario hands */
    GameBoardElementColor nextPillColorL, nextPillColorR;

    /* Gravity */
    GameSpeedProvider *speedProvider;
    uint32_t lastGravityTime;
    SinglePlayer_Action nextAction;
    bool lastActionResult;
    int currentPillId;

	/* Score logic */
	uint32_t deletedVirusCount;
	uint32_t scoreMultiplier;

    GameBoardElementColor lastVirusRemovedColor;
} SinglePlayerGame;

void SinglePlayerGame_init(this_p(SinglePlayerGame), size_t top, size_t level,
                           GameSpeedProvider *speedProvider, GameBoard *board);

#endif
