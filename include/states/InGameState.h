/*
 *  InGameState.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *  View part of a Single Player Game
 */

#ifndef INGAMESTATE_H
#define INGAMESTATE_H

#include "Object.h"
#include "GameState.h"
#include "Graphics.h"
#include "Engine.h"
#include "Assets.h"
#include "Bool.h"
#include "Sprite.h"
#include "SinglePlayerGame.h"
#include "Vector.h"

typedef class InGameState {
    GameState base;

	/* Timeline */


    /* Game logic */
    SinglePlayerGame logic;
	SinglePlayerGame_State lastLogicState;

    /* Sprites */
    Vector sprites;
    Sprite *marioSprite;
    Sprite *virusLargeBlueSprite, *virusLargeYellowSprite, *virusLargeRedSprite;

} InGameState;

void InGameState_init(this_p(InGameState), Engine *engine);

#endif
