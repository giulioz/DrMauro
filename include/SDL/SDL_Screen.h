/*
 *  SDL_Screen.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef SDLSCREEN_H
#define SDLSCREEN_H

#include <stdlib.h>
#include <SDL.h>

#include "Object.h"
#include "Bool.h"
#include "ExceptionManager.h"
#include "Graphics.h"
#include "Screen.h"
#include "SDL_Graphics.h"

typedef class SDL_Screen {
    Screen base;

    char* windowTitle;
    SDL_Window *window;
    SDL_Surface *screenSurface, *tempSurface;

    SDL_Graphics graphics;
} SDL_Screen;

void SDL_Screen_init(this_p(SDL_Screen), uint16_t width, uint16_t height, char* windowTitle, ScreenCallbacks* callbacks);

#endif
