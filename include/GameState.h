/*
 *  GameState.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Object.h"

class GameState;

struct GameState_VTABLE {
    void (*update)(this_p(GameState));
    void (*draw)(this_p(GameState));
    void (*load)(this_p(GameState));
    void (*unload)(this_p(GameState));
};

typedef class GameState {
    struct GameState_VTABLE *VTABLE;
    class Engine *engine;
} GameState;

#endif
