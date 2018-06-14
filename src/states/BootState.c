/*
 *  BootState.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "BootState.h"

static bool update(this_p(GameState)) {
    return true;
}

static void draw(this_p(GameState)) {
    Graphics *graphics = VTP(this->engine->screen)->getGraphics(this->engine->screen);

    /* Loading screen (nearly useless) */
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 10, 10, "Loading", 1);

    /* Load next state */
    MainMenuGameState nextState;
    MainMenuGameState_init(&nextState, this->engine);
    VTP(this->engine)->loadState(this->engine, (GameState*)&nextState);
}

static struct GameState_VTABLE _vtable = {
        update, draw
};

void BootState_init(this_p(BootState), Engine *engine) {
    this->base.engine = engine;
    VT(this->base) = &_vtable;
}
