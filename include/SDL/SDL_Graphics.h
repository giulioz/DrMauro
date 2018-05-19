/*
 *  SDL_Graphics.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef SDLGRAPHICS_H
#define SDLGRAPHICS_H

#include <stdlib.h>
#include <SDL2/SDL.h>

#include "Object.h"
#include "Bool.h"
#include "ExceptionManager.h"
#include "Graphics.h"

typedef struct SDL_Screen SDL_Screen;

typedef class SDL_Graphics {
    Graphics base;
    SDL_Screen* screen;
} SDL_Graphics;

void SDL_Graphics_init(this_p(SDL_Graphics), SDL_Screen* screen);

#endif
