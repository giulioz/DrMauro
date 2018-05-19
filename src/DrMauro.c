/*
 *  DrMauro.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  Game entry point
 */

#include <DrMauro.h>

static SDL_Screen screen;

//static void LoadAssets() {
    /* Asset Manager */
    //game.AssetManager = AssetManager_Create(AssetsBaseFolder);

    /* GUI */
    /*AssetManager_Add(&game.AssetManager, (BaseAsset*)Texture_LoadFromFileRGB_Keyed("panel1.png", &game.AssetManager, "\xFF\xFF\xFF\xFF"));
    AssetManager_Add(&game.AssetManager, (BaseAsset*)Texture_LoadFromFileRGB_Keyed("panel2.png", &game.AssetManager, "\xFF\xFF\xFF\xFF"));
    game.Panel1 = Panel3_Create((Texture*)AssetManager_Get(&game.AssetManager, "panel1.png"), 13, 1, 6);
    game.Panel2 = Panel3_Create((Texture*)AssetManager_Get(&game.AssetManager, "panel2.png"), 13, 1, 6);*/
//}

int main(int argc, char **argv) {
    BootState *bootState = BootState_new();
    SDL_Screen_init(&screen, 256, 240, "DR. MAURO");

    Engine_init();
    Engine_get()->startup((Screen*)&screen, (GameState *) bootState);
    
    return 0;
}
