/*
 *  Engine.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "Engine.h"

static void startup(this_p(Engine), Screen *screen, GameState *bootState) {
    this->screen = screen;
    VTP(this->screen)->init(this->screen);

    VTP(this)->loadState(this, bootState);
    VTP(this->screen)->run(this->screen);
}

static void shutdown(this_p(Engine)) {
    VTP(this->currentState)->unload(this->currentState);
    VTP(this->screen)->close(this->screen);
}

static void loadState(this_p(Engine), GameState *state) {
    /* unload old state */
    if (this->currentState)
        VTP(this->currentState)->unload(this->currentState);

    /* load new state */
    this->currentState = state;
    VTP(this->currentState)->load(this->currentState);
    this->screen->callbacks = (ScreenCallbacks *) state;
}

static struct Engine_VTABLE _vtable = {
        startup, shutdown, loadState
};

Engine *Engine_init(this_p(Engine)) {
    VTP(this) = &_vtable;

    this->screen = NULL;
    this->currentState = NULL;
    return this;
}
