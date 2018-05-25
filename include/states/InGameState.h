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

typedef class InGameState {
    GameState base;
} InGameState;

void InGameState_init(this_p(InGameState), Engine *engine);

#endif
