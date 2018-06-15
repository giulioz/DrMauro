/*
 *  SinglePlayerGameState.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  View part of a Single Player Game
 */

#ifndef SINGLEPLAYERGAMESTATE_H
#define SINGLEPLAYERGAMESTATE_H

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
#include "CommonGameUtils.h"

typedef class SinglePlayerGameState {
    GameState base;

	/* Timeline */
    Timeline timeline;

    /* Game logic */
    SinglePlayerGame *logic;
	SinglePlayerGame_State lastLogicState;

    /* Sprites */
    Sprite marioSprite;
    Sprite virusLargeBlueSprite, virusLargeYellowSprite, virusLargeRedSprite;

    /* Next pill */
    bool nextPillVisible;
    uint32_t nextPillLX, nextPillLY, nextPillRX, nextPillRY;
} SinglePlayerGameState;

void SinglePlayerGameState_init(this_p(SinglePlayerGameState), Engine *engine, SinglePlayerGame *logic);

#endif
