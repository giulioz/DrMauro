/*
 *  DrMauro.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  Game entry point
 */

#include <DrMauro.h>

const char* AssetsBaseFolder = "assets/";
static DrMauro game;

static void Update(Window* window, const double deltaTime) {

}

static void Draw(Window* window) {
    Graphics_Clear(window);
    Panel3_Draw(&game.Panel1, 10, 15, 150, window);
    Panel3_Draw(&game.Panel2, 140, 30, 70, window);
}

static void LoadAssets() {
    /* Asset Manager */
    game.AssetManager = AssetManager_Create(AssetsBaseFolder);

    /* GUI */
    AssetManager_Add(&game.AssetManager, (BaseAsset*)Texture_LoadFromFileRGB_Keyed("panel1.png", &game.AssetManager, "\xFF\xFF\xFF\xFF"));
    AssetManager_Add(&game.AssetManager, (BaseAsset*)Texture_LoadFromFileRGB_Keyed("panel2.png", &game.AssetManager, "\xFF\xFF\xFF\xFF"));
    game.Panel1 = Panel3_Create((Texture*)AssetManager_Get(&game.AssetManager, "panel1.png"), 13, 1, 6);
    game.Panel2 = Panel3_Create((Texture*)AssetManager_Get(&game.AssetManager, "panel2.png"), 13, 1, 6);
}

int main(int argc, char **argv) {
    /* Load Game */
    LoadAssets();

    /* Window */
    Window_Init(&game.Window, "DR. MAURO", 256, 240); /* Original NES Resolution */
    game.Window.Update = Update;
    game.Window.Draw = Draw;
    
    /* Run game */
    Window_Run(&game.Window);

    /* Exit and unload all */
    Window_End(&game.Window);
    AssetManager_UnloadAll(&game.AssetManager);
    
    return 0;
}
