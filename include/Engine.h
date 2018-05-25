/*
 *  Engine.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef ENGINE_H
#define ENGINE_H

#include "Object.h"
#include "Screen.h"
#include "GameState.h"

class Engine;

struct Engine_VTABLE {
    void (*startup)(this_p(Engine));
    void (*shutdown)(this_p(Engine));
    void (*loadState)(this_p(Engine), GameState *state);
    void (*forkState)(this_p(Engine), GameState *state);
};

typedef class Engine {
    struct Engine_VTABLE *VTABLE;

    /* === Engine components === */
    /* pointers to allow polymorphism */
    Screen *screen;
    GameState *currentState;
} Engine;

Engine *Engine_init(this_p(Engine), Screen *screen, GameState *bootState);

#endif
