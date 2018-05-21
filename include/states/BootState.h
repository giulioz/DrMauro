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

typedef class BootState {
    GameState base;
} BootState;

BootState* BootState_new(Engine *engine);

#endif
