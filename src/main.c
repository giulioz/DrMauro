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

int _main(int argc, char **argv) {
    /* Init components */
    BootState_init(&bootState, &engine);
    SDL_Screen_init(&screen, 256, 240, "DR. MAURO", NULL);

    /* Init and startup engine */
    Engine_init(&engine, (Screen*)&screen, (GameState *) &bootState);
    VT(engine)->startup(&engine);
    
    return 0;
}

/* STUB */
#ifdef WIN32
#include <windows.h>
int CALLBACK WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d) {
	return _main(0, NULL);
}
#else
int main(int argc, char **argv) {
	return _main(argc, argv);
}
#endif
