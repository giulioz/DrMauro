/*
 *  Window.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  Wraps around SDL, represents a generic game window
 */

#ifndef WINDOW_H
#define WINDOW_H

#include <stdlib.h>
#include <SDL2/SDL.h>

#include "Bool.h"
#include "ExceptionManager.h"

#define PressedKeys_Size 256

typedef struct Window {
    /* Graphics */
    SDL_Window *Window;
    SDL_Surface *ScreenSurface;

    /* Window state */
    bool Running;
    Uint32 LastTime;
    bool PressedKeys[PressedKeys_Size];

    /* Callbacks */
    void (*Update)(struct Window* game, const double deltaTime);
    void (*Draw)(struct Window* game);

} Window;

void Window_Init(Window* game, const char* title, const int width, const int height);
void Window_End(Window* game);
void Window_Run(Window* game);

#endif