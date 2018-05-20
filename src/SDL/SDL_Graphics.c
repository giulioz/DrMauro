/*
 *  SDL_Graphics.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "SDL/SDL_Graphics.h"
#include "ExceptionManager.h"
#include <string.h>
#include "SDL/SDL_Screen.h"

static Color* beginDraw(this_p(Graphics)) {
    SDL_Screen* screen = ((SDL_Graphics*)this)->screen;
    SDL_LockSurface(screen->screenSurface);
    return screen->screenSurface->pixels;
}

static void endDraw(this_p(Graphics)) {
    SDL_UnlockSurface(((SDL_Graphics*)this)->screen->screenSurface);
}

static void drawCheckerboard(this_p(Graphics), int step, Color colorA, Color colorB) {
    SDL_Graphics* graphics = (SDL_Graphics*)this;
    int x, y, runningX = 0, runningY = 0, state = 1, i = 0;
    Color* framebuffer = beginDraw(this);
    for (y = 0; y < ((Screen*)graphics->screen)->height; y++) {
        for (x = 0; x < ((Screen*)graphics->screen)->width; x++) {
            if (runningX >= step) {
                runningX = 0;
                state = !state;
            }
            runningX++;
            if (state) framebuffer[i] = colorA;
            else framebuffer[i] = colorB;
            i++;
        }
        if (runningY >= step) {
            runningY = 0;
            state = !state;
        }
        runningY++;
    }
    endDraw(this);
}

static struct Graphics_VTABLE _vtable = {
        drawCheckerboard
};

void SDL_Graphics_init(this_p(SDL_Graphics), SDL_Screen* screen) {
    VT(this->base) = &_vtable;
    this->screen = screen;
}
