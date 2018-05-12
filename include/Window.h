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
#include "AssetManager.h"

#define PressedKeys_Size 256

typedef struct Window {
    /* Graphics internals */
    SDL_Window *Window;
    SDL_Surface *ScreenSurface;

    /* Window state */
    int WindowWidth, WindowHeight;
    bool Running;
    Uint32 LastTime;
    bool PressedKeys[PressedKeys_Size];

    /* Callbacks */
    void (*Update)(struct Window* window, const double deltaTime);
    void (*Draw)(struct Window* window);

} Window;

void Window_Init(Window* window, const char* title, int width, int height);
void Window_End(Window* window);
void Window_Run(Window* window);
unsigned char* Window_GetFramebuffer(Window* window);

#endif
