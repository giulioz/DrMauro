/*
 *  DrMauro.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  Game entry point
 */

#include "DrMauro.h"

static void Update(Window* game, const double deltaTime) {

}

static void Draw(Window* game) {

}

int main(int argc, char **argv) {
    Window game;
    Window_Init(&game, "DR. MAURO", 256, 240); /* Original NES Resolution */
    game.Update = Update;
    game.Draw = Draw;
    Window_Run(&game);
    
    return 0;
}
