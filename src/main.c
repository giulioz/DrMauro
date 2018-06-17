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
static SDL_InputDevice inputDevice;
static Parameters parameters;
static BootState bootState;

int _main() {
    SDL_InputDevice_init(&inputDevice);
    SDL_Screen_init(&screen, 256, 240, "DR. MAURO", &inputDevice);
    Engine_init(&engine, (Screen*)&screen, (InputDevice *) &inputDevice, &parameters);
    BootState_init(&bootState, &engine);

    VT(engine)->startup(&engine, (GameState *) &bootState);

    return EXIT_SUCCESS;
}

/* STUB */
#ifdef WIN32
#include <windows.h>
int CALLBACK WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d) {
	Parameters_init(&parameters);
	return _main(0, NULL);
}
#else
int main(int argc, char **argv) {
	Parameters_init(&parameters);
	VT(parameters)->parseParameters(&parameters, argc, argv);
	return _main();
}
#endif
