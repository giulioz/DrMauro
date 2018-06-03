/*
 *  SDL_Screen.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "SDL_Screen.h"

#define scaling 2

static void init(this_p(Screen)) {
    SDL_Screen *screen = (SDL_Screen*)this;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        ThrowError("SDL_INIT failed!");
    }

    screen->window = SDL_CreateWindow(
            screen->windowTitle,                                    /* title */
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,       /* position */
            this->width*scaling, this->height*scaling,              /* size */
            SDL_WINDOW_SHOWN                                        /* flags */
    );

    if (!screen->window) ThrowError("SDL_CreateWindow failed!");
    screen->tempSurface = SDL_GetWindowSurface(screen->window);
    screen->screenSurface = SDL_CreateRGBSurface(0, this->width, this->height, 32, 0, 0, 0, 0);
}

static void run(this_p(Screen)) {
    SDL_Rect dstRect = { 0, 0, 0, 0 };
    dstRect.w = ((SDL_Screen*)this)->tempSurface->w;
    dstRect.h = ((SDL_Screen*)this)->tempSurface->h;
    this->running = true;

    /* event loop */
    while (this->running) {

        /* pool Events */
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    this->running = false;
                    break;
                case SDL_KEYUP:
                    break;
                case SDL_KEYDOWN:
                    break;
                default:
                    break;
            }
        }

        /* callbacks */
        VTP(this->callbacks)->update(this->callbacks);
        VTP(this->callbacks)->draw(this->callbacks);
        SDL_BlitScaled(((SDL_Screen*)this)->screenSurface, NULL, ((SDL_Screen*)this)->tempSurface, &dstRect);
        SDL_UpdateWindowSurface(((SDL_Screen*)this)->window);

        /* HACK: frame limiter */
        SDL_Delay(10);
    }
}

static void close(this_p(Screen)) {
    SDL_Screen *screen = (SDL_Screen*)this;

    this->running = false;
    SDL_FreeSurface(screen->screenSurface);
    SDL_FreeSurface(screen->tempSurface);
    SDL_DestroyWindow(screen->window);
    SDL_Quit();
}

static Graphics* getGraphics(this_p(Screen)) {
    SDL_Screen *screen = ((SDL_Screen*)this);
    return (Graphics *) &(screen->graphics);
}

static uint32_t getCurrentTime(this_p(Screen)) {
    return SDL_GetTicks();
}

static struct Screen_VTABLE _vtable = {
        init, run, close, getGraphics, getCurrentTime
};

/* ********************************************************* */
/* Constructor                                               */
void SDL_Screen_init(this_p(SDL_Screen), uint16_t width, uint16_t height, char* windowTitle, ScreenCallbacks* callbacks) {
    VT(this->base) = &_vtable;
    this->base.callbacks = callbacks;

    this->base.width = width;
    this->base.height = height;
    this->base.running = false;
    this->windowTitle = windowTitle;
    SDL_Graphics_init(&this->graphics, this);
}
