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
#include "IntTypes.h"
#include "Timeline.h"
#include "GameBoard.h"

typedef class SinglePlayerGameState {
    GameState base;

	/* Timeline */
    Timeline timeline;

    /* Game logic */
    SinglePlayerGame logic;
	SinglePlayerGame_State lastLogicState;

    /* Sprites */
    Sprite marioSprite;
    Sprite virusLargeBlueSprite, virusLargeYellowSprite, virusLargeRedSprite;

} SinglePlayerGameState;

void SinglePlayerGameState_init(this_p(SinglePlayerGameState), Engine *engine, int top, int level, int virus, SinglePlayerGame_Speed speed);

#endif
