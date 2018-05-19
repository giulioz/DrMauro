/*
 *  GameState.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Object.h"

typedef class GameState {
    void (*load)();
    void (*unload)();
    void (*update)(double deltaTime);
    void (*draw)();
} GameState;

#endif
