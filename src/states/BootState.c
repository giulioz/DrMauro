/*
 *  BootState.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "states/BootState.h"

static void load(this_p(GameState)) {

}

static void unload(this_p(GameState)) {

}

static void update(this_p(GameState), double deltaTime) {

}

static void draw(this_p(GameState)) {
    Graphics *graphics = VTP(this->engine->screen)->getGraphics(this->engine->screen);

    /* Background */
    VTP(graphics)->drawCheckerboard(graphics, 8, Color_FromInt(0x4C00AB), Color_FromInt(0x000000));
    /*Panel3_draw(&game.Panel1, 8, 34, 59, window);
    Panel3_draw(&game.Panel2, 176, 122, 83, window);*/
}

static struct GameState_VTABLE _vtable = {
        update, draw, load, unload
};

BootState* BootState_new(Engine *engine) {
    BootState* bootState = new(BootState);
    bootState->base.engine = engine;
    VT(bootState->base) = &_vtable;
    return bootState;
}
