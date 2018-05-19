/*
 *  Engine.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "Engine.h"

/* singleton */
static Engine _engine;
static Engine *this;

static void startup(Screen *screen, GameState *bootState) {
    this->screen = screen;
    this->screen->init(this->screen);

    this->loadState(bootState);
    this->screen->run(this->screen);
}

static void shutdown() {
    this->currentState->unload();
    this->screen->close(this->screen);
}

static void loadState(GameState *state) {
    /* unload old state */
    if (this->currentState)
        this->currentState->unload();

    /* load new state */
    this->currentState = state;
    this->currentState->load();
    this->screen->update = this->currentState->update;
    this->screen->draw = this->currentState->draw;
}

Engine *Engine_init() {
    this = &_engine;
    this->startup = startup;
    this->shutdown = shutdown;
    this->loadState = loadState;

    this->screen = NULL;
    this->currentState = NULL;
    return this;
}

Engine *Engine_get() {
    return this;
}
