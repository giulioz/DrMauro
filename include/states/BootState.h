/*
 *  BootState.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef BOOTSTATE_H
#define BOOTSTATE_H

#include "Object.h"
#include "GameState.h"
#include "Graphics.h"
#include "Engine.h"
#include "assets/Assets.h"
#include "InGameState.h"

typedef class BootState {
    GameState base;

    InGameState inGameState;
} BootState;

void BootState_init(this_p(BootState), Engine *engine);

#endif
