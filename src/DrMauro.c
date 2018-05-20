/*
 *  DrMauro.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  Game entry point
 */

#include "DrMauro.h"

static Engine engine;
static SDL_Screen screen;

int main(int argc, char **argv) {
    BootState *bootState = BootState_new(&engine);
    SDL_Screen_init(&screen, 256, 240, "DR. MAURO", NULL);

    Engine_init(&engine);
    VT(engine)->startup(&engine, (Screen*)&screen, (GameState *) bootState);
    
    return 0;
}
