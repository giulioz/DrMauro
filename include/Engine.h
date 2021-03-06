/*
 *  Engine.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef ENGINE_H
#define ENGINE_H

#include "Object.h"
#include "Screen.h"
#include "GameState.h"
#include "InputDevice.h"
#include "Parameters.h"

class Engine;

struct Engine_VTABLE {
    void (*startup)(this_p(Engine), GameState *bootState);
    void (*shutdown)(this_p(Engine));
    void (*loadState)(this_p(Engine), GameState *state);
};

typedef class Engine {
    struct Engine_VTABLE *VTABLE;

    /* === Engine components === */
    /* pointers to allow polymorphism */
    Screen *screen;
    InputDevice *inputDevice;
    Parameters *parameters;
} Engine;

Engine *Engine_init(this_p(Engine), Screen *screen, InputDevice *inputDevice, Parameters *parameters);

#endif
