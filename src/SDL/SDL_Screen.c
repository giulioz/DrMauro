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

static void setKey(this_p(SDL_Screen), SDL_Event *e, bool value) {
    switch (e->key.keysym.sym) {
        /* Player 1 */
        case SDLK_LEFT:
            this->inputDevice->currentState.leftButton = value;
            break;
        case SDLK_RIGHT:
            this->inputDevice->currentState.rightButton = value;
            break;
        case SDLK_UP:
            this->inputDevice->currentState.upButton = value;
            break;
        case SDLK_DOWN:
            this->inputDevice->currentState.downButton = value;
            break;
        case SDLK_m:
            this->inputDevice->currentState.rotateRightButton = value;
            break;
        case SDLK_n:
            this->inputDevice->currentState.rotateLeftButton = value;
            break;

        /* Player 2 */
        case SDLK_a:
            this->inputDevice->currentState.leftButton2 = value;
            break;
        case SDLK_d:
            this->inputDevice->currentState.rightButton2 = value;
            break;
        case SDLK_w:
            this->inputDevice->currentState.upButton2 = value;
            break;
        case SDLK_s:
            this->inputDevice->currentState.downButton2 = value;
            break;
        case SDLK_1:
            this->inputDevice->currentState.rotateRightButton2 = value;
            break;
        case SDLK_2:
            this->inputDevice->currentState.rotateLeftButton2 = value;
            break;


        case SDLK_RETURN:
            this->inputDevice->currentState.enterButton = value;
            break;
        default:
            break;
    }
}

static void run(this_p(Screen)) {
    SDL_Screen *screen = (SDL_Screen *) this;

    SDL_Rect dstRect = { 0, 0, 0, 0 };
    dstRect.w = screen->tempSurface->w;
    dstRect.h = screen->tempSurface->h;
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
                    setKey(screen, &event, false);
                    break;
                case SDL_KEYDOWN:
                    setKey(screen, &event, true);
                    break;
                default:
                    break;
            }
        }

        /* callbacks */
        VTP(this->callbacks)->update(this->callbacks);
        VTP(this->callbacks)->draw(this->callbacks);
        SDL_BlitScaled(screen->screenSurface, NULL, screen->tempSurface, &dstRect);
        SDL_UpdateWindowSurface(screen->window);

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
void SDL_Screen_init(this_p(SDL_Screen), uint16_t width, uint16_t height, char* windowTitle, ScreenCallbacks* callbacks, SDL_InputDevice *inputDevice) {
    VT(this->base) = &_vtable;
    this->base.callbacks = callbacks;

    this->base.width = width;
    this->base.height = height;
    this->base.running = false;
    this->windowTitle = windowTitle;
    this->inputDevice = inputDevice;
    SDL_Graphics_init(&this->graphics, this);
}
