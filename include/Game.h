/*
 *  Game.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  Wraps around SDL, represents a generic game
 */

#ifndef GAME_H
#define GAME_H

#include <stdlib.h>
#include <SDL2/SDL.h>

#include "Bool.h"
#include "ExceptionManager.h"

typedef struct Game {
    /* Graphics */
    SDL_Window *Window;
    SDL_Surface *ScreenSurface;

    /* Game state */
    bool Running;
    Uint32 LastTime;

    /* Callbacks */
    void (*Update)(struct Game* game, const double deltaTime);
    void (*Draw)(struct Game* game);

} Game;

void Game_Init(Game* game, const char* title, const int width, const int height);
void Game_End(Game* game);
void Game_Run(Game* game);

#endif
