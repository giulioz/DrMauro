/*
 *  Engine.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "Engine.h"

/* Startup all the components */
static void startup(this_p(Engine)) {
    VTP(this->screen)->init(this->screen);

    VTP(this)->loadState(this, this->currentState);
    VTP(this->screen)->run(this->screen);
}

/* Shutdown all the components */
static void shutdown(this_p(Engine)) {
    VTP(this->currentState)->unload(this->currentState);
    VTP(this->screen)->close(this->screen);
}

/* Unload old state and load another one */
static void loadState(this_p(Engine), GameState *state) {
    /* unload old state */
    if (this->currentState) {
        VTP(this->currentState)->unload(this->currentState);
        VTP(this->inputDevice)->reset(this->inputDevice);
    }

    /* load new state */
    this->currentState = state;
    VTP(this->currentState)->load(this->currentState);
    this->screen->callbacks = (ScreenCallbacks *) state;
}


static struct Engine_VTABLE _vtable = {
        startup, shutdown, loadState
};

Engine *Engine_init(this_p(Engine), Screen *screen, GameState *bootState, InputDevice* inputDevice) {
    VTP(this) = &_vtable;

    this->screen = screen;
    this->currentState = bootState;
    this->inputDevice = inputDevice;
    return this;
}
