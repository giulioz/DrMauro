/*
 *  main.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  Game entry point
 */

#define OPTPARSE_IMPLEMENTATION
#include "main.h"

/* Engine and components allocation */
static Engine engine;
static SDL_Screen screen;
static SDL_InputDevice inputDevice;

/* Usage print */
void usage() {
    printf( "DR.MAURO - dr.Mario Clone                        \n"
            "Usage: drmauro [-f FILE | -d DIFFICULTY] [-s SPEED] [-h] \n"
            "                                                         \n"
            "OPTIONS:                                                 \n"
            "  -f FILE         Load board from FILE                   \n"
            "  -d DIFFICULTY   Generate random board (default 5)      \n"
            "  -s SPEED        Game speed (0, 1 or 2)                 \n" /* speed has been modified because accepting custom values was too difficult, sorry */
            "  -h              Show this help message                 \n"
    );
}

void startGameMenu() {
    BootState bootState;
    BootState_init(&bootState, &engine);
    Engine_init(&engine, (Screen*)&screen, (GameState *) &bootState, (InputDevice *) &inputDevice);
    VT(engine)->startup(&engine);
}

void jumpToGame(int difficulty, int speed) {
    SinglePlayerGameState singlePlayerGameState;
    SinglePlayerGame logic;
    GameBoard board;
    Engine_init(&engine, (Screen*)&screen, (GameState *) &singlePlayerGameState, (InputDevice *) &inputDevice);
    GameBoard_init(&board);
    SinglePlayerGame_init(&logic, &engine, 0, 0, difficulty, (SinglePlayerGame_Speed) speed, &board);
    SinglePlayerGameState_init(&singlePlayerGameState, &engine, &logic);
    VT(engine)->startup(&engine);
}

int _main(int argc, char **argv) {
    struct optparse options;
    int option;

    int difficulty = 0;
    int speed = 0;
    char* boardFile = NULL;

    SDL_InputDevice_init(&inputDevice);
    SDL_Screen_init(&screen, 256, 240, "DR. MAURO", &inputDevice);

    /* normal game */
    if (argc <= 1) {
        startGameMenu();
        return EXIT_SUCCESS;
    }

    optparse_init(&options, argv);
    while ((option = optparse(&options, "f:d:s:h")) != -1) {
        switch (option) {
            case 'f':
                boardFile = options.optarg;
                break;
            case 'd':
                difficulty = atoi(options.optarg);
                break;
            case 's':
                speed = atoi(options.optarg);
                break;
            case 'h':
                usage();
                return EXIT_FAILURE;
            default:
                break;
        }
    }

    if (boardFile) {

    } else {
        jumpToGame(difficulty, speed);
    }

    return EXIT_SUCCESS;
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
