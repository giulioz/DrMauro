/*
 *  Window.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  Wraps around SDL, represents a generic window window
 */

#include "Window.h"

void Window_Init(Window* window, const char* title, const int width, const int height) {
    /* Clear class fields */
    int i;
    window->Running = false;
    window->Update = NULL;
    window->Draw = NULL;
    for (i = 0; i < PressedKeys_Size; i++) window->PressedKeys[i] = false;
    window->WindowWidth = width;
    window->WindowHeight = height;

    /* Starts SDL */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        ThrowError("SDL_INIT failed!");
    }

    /* Create Window and Surface */
    window->Window = SDL_CreateWindow(
        title,                                                  /* title */
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,       /* position */
        width, height,                                          /* size */
        SDL_WINDOW_SHOWN                                        /* flags */
    );
    if (!window->Window) ThrowError("SDL_CreateWindow failed!");
    window->ScreenSurface = SDL_GetWindowSurface(window->Window);
}

void Window_End(Window* window) {
    window->Running = false;

    /* Clear SDL */
    SDL_FreeSurface(window->ScreenSurface);
    SDL_DestroyWindow(window->Window);
    SDL_Quit();
}

void Window_Run(Window* window) {
    if (!window->Update) ThrowError("No update function defined!");
    if (!window->Draw) ThrowError("No draw function defined!");

    window->Running = true;
    window->LastTime = SDL_GetTicks();

    /* Main cycle */
    while (window->Running) {
        Uint32 currentTime = SDL_GetTicks();
        double deltaTime = (currentTime - window->LastTime) / (double)1000;

        /* Pool Events */
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    window->Running = false;
                    break;
                case SDL_KEYUP:
                    //window->PressedKeys[event.key.keysym.sym] = false;
                    break;
                case SDL_KEYDOWN:
                    //window->PressedKeys[event.key.keysym.sym] = true;
                    break;
            }
        }

        /* Callbacks */
        window->Update(window, deltaTime);
        SDL_LockSurface(window->ScreenSurface);
        window->Draw(window);
        SDL_UnlockSurface(window->ScreenSurface);
        SDL_UpdateWindowSurface(window->Window);

        window->LastTime = SDL_GetTicks();
    }
}

unsigned char* Window_GetFramebuffer(Window* window) {
    return window->ScreenSurface->pixels;
}
