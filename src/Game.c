/*
 *  Game.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  Wraps around SDL, represents a generic game
 */

#include "Game.h"

void Game_Init(Game* game, const char* title, const int width, const int height) {
    game->Running = false;
    game->Update = NULL;
    game->Draw = NULL;

    /* Starts SDL */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        ThrowError("SDL_INIT failed!");
    }

    /* Create Window and Surface */
    game->Window = SDL_CreateWindow(
        title,                                                  /* title */
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,       /* position */
        width, height,                                          /* size */
        SDL_WINDOW_SHOWN                                        /* flags */
    );
    if (!game->Window) ThrowError("SDL_CreateWindow failed!");
    game->ScreenSurface = SDL_GetWindowSurface(game->Window);
}

void Game_End(Game* game) {
    game->Running = false;

    /* Clear SDL */
    SDL_FreeSurface(game->ScreenSurface);
    SDL_DestroyWindow(game->Window);
    SDL_Quit();
}

void Game_Run(Game* game) {
    if (!game->Update) ThrowError("No update function defined!");
    if (!game->Draw) ThrowError("No draw function defined!");

    game->Running = true;
    game->LastTime = SDL_GetTicks();

    /* Main cycle */
    while (game->Running) {
        Uint32 currentTime = SDL_GetTicks();
        double deltaTime = (currentTime - game->LastTime) / (double)1000;

        /* Pool Events */
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    game->Running = false;
                    break;
                case SDL_KEYUP:
                    break;
            }
        }

        game->Update(game, deltaTime);
        game->Draw(game);
        game->LastTime = SDL_GetTicks();
    }
}
