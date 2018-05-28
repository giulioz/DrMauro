/*
 *  InGameState.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
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

typedef class InGameState {
    GameState base;

    Sprite marioSprite;
    Sprite virusLargeBlueSprite, virusLargeYellowSprite, virusLargeRedSprite;
} InGameState;

void InGameState_init(this_p(InGameState), Engine *engine);

#endif
