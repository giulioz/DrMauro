/*
 *  Game.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  Wraps around SDL, represents a generic game
 */

#include "Game.h"

void Game_Init(Game* game, const char* title, const int width, const int height) {
    /* Init struct variables */
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
    /* Clear SDL */
    SDL_FreeSurface(game->ScreenSurface);
    SDL_Quit();
}

void Game_Run(Game* game) {
    game->Running = true;
    game->LastTime = SDL_GetTicks();

    while (game->Running) {
        Uint32 currentTime = SDL_GetTicks();
        double deltaTime = (currentTime - game->LastTime) / (double)1000;
        game->Update(game, deltaTime);
        game->Draw(game);
        game->LastTime = SDL_GetTicks();
    }
}
