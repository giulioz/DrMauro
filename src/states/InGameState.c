/*
 *  InGameState.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "SDL_Screen.h"
#include "InGameState.h"

/* *************************************************************** */
/* Internals                                                       */

static void drawScorePanel(this_p(GameState), Graphics* graphics, int top, int score) {
    char top_c[8] = {0}, score_c[8] = {0};
    sprintf(top_c, "%07d", top);
    sprintf(score_c, "%07d", score);

    VT(Asset_PanelLarge)->draw(&Asset_PanelLarge, graphics, 8, 34, 59);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 16, 55, "TOP");
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 16, 63, top_c);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 16, 79, "SCORE");
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 16, 87, score_c);
}

static void drawLevelPanel(this_p(GameState), Graphics* graphics, int level, int speed, int virus) {
    char level_c[3] = {0}, virus_c[3] = {0};
    sprintf(level_c, "%02d", level);
    sprintf(virus_c, "%02d", virus);

    VT(Asset_PanelSmall)->draw(&Asset_PanelSmall, graphics, 176, 122, 83);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 184, 143, "LEVEL");
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 216, 151, level_c);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 184, 167, "SPEED");
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 208, 175, "MED");
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 184, 191, "VIRUS");
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 216, 199, virus_c);
}

/* *************************************************************** */
/* Interface                                                       */

static void load(this_p(GameState)) {

}

static void unload(this_p(GameState)) {

}

static void update(this_p(GameState), double deltaTime) {

}

static void draw(this_p(GameState)) {
    Graphics *graphics = VTP(this->engine->screen)->getGraphics(this->engine->screen);
    graphics->currentPalette = &Asset_DefaultPalette;

    /* Background */
    VTP(graphics)->drawCheckerboard(graphics, 8, 2, 0);
    VTP(graphics)->drawTexture(graphics, &Asset_Logo, 159, 26, 159 + Asset_Logo.width, 26 + Asset_Logo.height, 0, 0, Asset_Logo.width, Asset_Logo.height);
    VT(Asset_PanelBottle)->draw(&Asset_PanelBottle, graphics, 88, 40, 128);
    VT(Asset_MarioBox)->draw(&Asset_MarioBox, graphics, 176, 64, 38, 38);

    /* Panels */
    drawScorePanel(this, graphics, 10000, 200);
    drawLevelPanel(this, graphics, 0, 5, 02);

    /*SDL_SaveBMP(((SDL_Graphics*)graphics)->screen->screenSurface, "a.bmp");
    ThrowError("debug");*/
}


/* *************************************************************** */
/* Initalization                                                   */

static struct GameState_VTABLE _vtable = {
        update, draw, load, unload
};

void InGameState_init(this_p(InGameState), Engine *engine) {
    this->base.engine = engine;
    VT(this->base) = &_vtable;
}
