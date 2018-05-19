/*
 *  Engine.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef ENGINE_H
#define ENGINE_H

#include "Object.h"
#include "Screen.h"
#include "GameState.h"

typedef class Engine {
    void (*startup)(Screen *screen, GameState *bootState);
    void (*shutdown)();
    void (*loadState)(GameState *state);

    Screen *screen;
    GameState *currentState;
} Engine;

/* singleton */
Engine* Engine_init();
Engine *Engine_get();

#endif
