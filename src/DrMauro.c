/*
 *  DrMauro.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  Game entry point
 */

#include "DrMauro.h"

static void Update(Game* game, const double deltaTime) {

}

static void Draw(Game* game) {

}

int main(int argc, char **argv) {
    Game game;
    Game_Init(&game, "DR. MAURO", 256, 240); /* Original NES Resolution */
    game.Update = Update;
    game.Draw = Draw;
    Game_Run(&game);
    
    return 0;
}
