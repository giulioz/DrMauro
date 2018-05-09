#include "DrMauro.h"

int main(int argc, char **argv) {
    Game game;
    Game_Init(&game, "DR. MAURO", 256, 240); // Original NES Resolution
    Game_Run(&game);
}
