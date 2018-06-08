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
static SDL_InputDevice inputDevice;

/* Usage pring */
void usage() {
    printf( "DR.MAURO - dr.Mario Clone                        \n"
            "Usage: drmauro [-f FILE | -d DIFFICULTY] [-s SPEED] [-h] \n"
            "                                                         \n"
            "OPTIONS:                                                 \n"
            "  -f FILE         Load board from FILE                   \n"
            "  -d DIFFICULTY   Generate random board (default 5)      \n"
            "  -s SPEED        Game speed (default 0.3 sec)           \n"
            "  -h              Show this help message                 \n"
    );
}

int _main(int argc, char **argv) {
    /* Init components */
    BootState_init(&bootState, &engine);
    SDL_InputDevice_init(&inputDevice);
    SDL_Screen_init(&screen, 256, 240, "DR. MAURO", NULL, &inputDevice);

    /* Init and startup engine */
    Engine_init(&engine, (Screen*)&screen, (GameState *) &bootState, (InputDevice *) &inputDevice);
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
