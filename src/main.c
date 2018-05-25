/*
 *  main.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  Game entry point
 */

#include "main.h"

/* Engine and components allocation */
static Engine engine;
static SDL_Screen screen;
static BootState bootState;

int main(int argc, char **argv) {
    /* Init components */
    BootState_init(&bootState, &engine);
    SDL_Screen_init(&screen, 256, 240, "DR. MAURO", NULL);

    /* Init and startup engine */
    Engine_init(&engine, (Screen*)&screen, (GameState *) &bootState);
    VT(engine)->startup(&engine);
    
    return 0;
}
