/*
 *  BootState.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "BootState.h"

static void load(this_p(GameState)) {

}

static void unload(this_p(GameState)) {

}

static void update(this_p(GameState)) {

}

static void draw(this_p(GameState)) {
    Graphics *graphics = VTP(this->engine->screen)->getGraphics(this->engine->screen);

    /* Loading screen (nearly useless) */
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 10, 10, "Loading", 1);

    /* Load next state */
    MainMenuGameState_init(&((BootState*)this)->mainMenuGameState, this->engine);
    VTP(this->engine)->forkState(this->engine, (GameState*)&((BootState*)this)->mainMenuGameState);

    /*AudioTestState_init(&((BootState*)this)->audioTestState, this->engine);
    VTP(this->engine)->forkState(this->engine, (GameState*)&((BootState*)this)->audioTestState);*/
}

static struct GameState_VTABLE _vtable = {
        update, draw, load, unload
};

void BootState_init(this_p(BootState), Engine *engine) {
    this->base.engine = engine;
    VT(this->base) = &_vtable;
}
