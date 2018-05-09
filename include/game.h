#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "Bool.h"

typedef struct {
    // Graphics
    SDL_Window *Window;
    SDL_Surface *ScreenSurface;

    // Game state
    bool Running;

} Game;

#endif
