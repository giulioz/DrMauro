/*
 *  BootState.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "BootState.h"

/* Static resources */
static Color whiteColor = { 0xFF, 0xFF, 0xFF, 0xFF };

static void load(this_p(GameState)) {

}

static void unload(this_p(GameState)) {

}

static void update(this_p(GameState), double deltaTime) {

}

static void draw(this_p(GameState)) {
    Graphics *graphics = VTP(this->engine->screen)->getGraphics(this->engine->screen);

    /* Loading screen (nearly useless) */
    VTP(graphics)->fill(graphics, whiteColor);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 10, 10, "Loading");

    /* Load next state */
    InGameState_init(&((BootState*)this)->inGameState, this->engine);
    VTP(this->engine)->forkState(this->engine, (GameState*)&((BootState*)this)->inGameState);
}

static struct GameState_VTABLE _vtable = {
        update, draw, load, unload
};

void BootState_init(this_p(BootState), Engine *engine) {
    this->base.engine = engine;
    VT(this->base) = &_vtable;
}
