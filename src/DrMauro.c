/*
 *  DrMauro.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  Game entry point
 */

#include <DrMauro.h>

const char* AssetsBaseFolder = "assets/";

static void Update(Window* game, const double deltaTime) {

}

static void Draw(Window* game) {

}

static void LoadAssets(DrMauro* game) {
    /* Asset Manager */
    game->AssetManager = AssetManager_Create(AssetsBaseFolder);

    /* GUI */
    AssetManager_Add(&game->AssetManager, (BaseAsset*)Texture_LoadFromFileRGB("panel1.png", &game->AssetManager));
}

int main(int argc, char **argv) {
    DrMauro game;

    /* Window */
    Window_Init(&game.Window, "DR. MAURO", 256, 240); /* Original NES Resolution */
    game.Window.Update = Update;
    game.Window.Draw = Draw;
    
    /* Assets */
    LoadAssets(&game);
    
    /* Run game */
    Window_Run(&game.Window);
    
    return 0;
}
