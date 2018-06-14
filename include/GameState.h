/*
 *  GameState.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Object.h"
#include "Bool.h"

class GameState;

struct GameState_VTABLE {
    bool (*update)(this_p(GameState)); /* returns false if state ended */
    void (*draw)(this_p(GameState));
};

typedef class GameState {
    struct GameState_VTABLE *VTABLE;
    class Engine *engine;
} GameState;

#endif
