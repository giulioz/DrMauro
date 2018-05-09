#include "DrMauro.h"

static void Update(const float deltaTime) {

}

static void Draw() {

}

int main(int argc, char **argv) {
    Game game;
    Game_Init(&game, "DR. MAURO", 256, 240); /* Original NES Resolution */
    game.Update = Update;
    game.Draw = Draw;
    Game_Run(&game);
}
