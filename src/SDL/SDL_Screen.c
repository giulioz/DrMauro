/*
 *  SDL_Screen.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "SDL_Screen.h"

static void init(this_p(Screen)) {
    SDL_Screen *screen = (SDL_Screen*)this;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        ThrowError("SDL_INIT failed!");
    }

    screen->window = SDL_CreateWindow(
            screen->windowTitle,                                    /* title */
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,       /* position */
            this->width, this->height,                              /* size */
            SDL_WINDOW_SHOWN                                        /* flags */
    );

    if (!screen->window) ThrowError("SDL_CreateWindow failed!");
    screen->screenSurface = SDL_GetWindowSurface(screen->window);
}

static void run(this_p(Screen)) {
    this->running = true;
    this->lastTime = SDL_GetTicks();

    /* event loop */
    while (this->running) {
        Uint32 currentTime = SDL_GetTicks();
        double deltaTime = (currentTime - this->lastTime) / (double)1000;

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
        VTP(this->callbacks)->update(this->callbacks, deltaTime);
        VTP(this->callbacks)->draw(this->callbacks);
        SDL_UpdateWindowSurface(((SDL_Screen*)this)->window);

        /* HACK: lowers battery usage */
        SDL_Delay(10);
        this->lastTime = SDL_GetTicks();
    }
}

static void close(this_p(Screen)) {
    SDL_Screen *screen = (SDL_Screen*)this;

    this->running = false;
    SDL_FreeSurface(screen->screenSurface);
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
    this->base.lastTime = 0;
    this->base.running = false;
    this->windowTitle = windowTitle;
    SDL_Graphics_init(&this->graphics, this);
}
