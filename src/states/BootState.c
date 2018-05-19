/*
 *  BootState.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "states/BootState.h"
#include "Graphics.h"

static void load() {

}

static void unload() {

}

static void update(double deltaTime) {

}

static void draw() {
    /* Background */
    Graphics_drawCheckerboard(8, 0x4C00AB, 0x000000);
    /*Panel3_draw(&game.Panel1, 8, 34, 59, window);
    Panel3_draw(&game.Panel2, 176, 122, 83, window);*/
}

BootState* BootState_new() {
    BootState* bootState = new(BootState);
    bootState->base.load = load;
    bootState->base.unload = unload;
    bootState->base.update = update;
    bootState->base.draw = draw;
    return bootState;
}
