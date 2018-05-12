/*
 *  DrMauro.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  Game entry point
 */

#include <DrMauro.h>
#include <Font.h>

const char* AssetsBaseFolder = "assets/";
static DrMauro game;

static void Update(Window* window, const double deltaTime) {

}

static void Draw(Window* window) {
    Texture* testTexture = (Texture*)AssetManager_Get(&game.AssetManager, "panel1.png");
    unsigned char* framebuffer = Window_GetFramebuffer(window);
    for (int y = 0; y < testTexture->Height; ++y) {
        for (int x = 0; x < testTexture->Width; ++x) {
            framebuffer[(x + y * window->WindowWidth)*4+2] = ((unsigned char*)testTexture->TextureData)[(x + y * testTexture->Width)*3];
            framebuffer[(x + y * window->WindowWidth)*4+1] = ((unsigned char*)testTexture->TextureData)[(x + y * testTexture->Width)*3+1];
            framebuffer[(x + y * window->WindowWidth)*4] = ((unsigned char*)testTexture->TextureData)[(x + y * testTexture->Width)*3+2];
        }
    }
}

static void LoadAssets(DrMauro* game) {
    /* Asset Manager */
    game->AssetManager = AssetManager_Create(AssetsBaseFolder);

    /* GUI */
    AssetManager_Add(&game->AssetManager, (BaseAsset*)Texture_LoadFromFileRGB("panel1.png", &game->AssetManager));
}

int main(int argc, char **argv) {
    /* Load Game */
    LoadAssets(&game);

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
