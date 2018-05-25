/*
 *  InGameState.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "InGameState.h"

#include "SDL_Graphics.h"
#include "SDL_Screen.h"

static Color checkerboardColorA = { 164, 0, 59, 0 };
static Color checkerboardColorB = { 0, 0, 0, 0 };

static void load(this_p(GameState)) {

}

static void unload(this_p(GameState)) {

}

static void update(this_p(GameState), double deltaTime) {

}

static void draw(this_p(GameState)) {
    Graphics *graphics = VTP(this->engine->screen)->getGraphics(this->engine->screen);

    /* Background */
    VTP(graphics)->drawCheckerboard(graphics, 8, checkerboardColorA, checkerboardColorB);

    /* Panels */
    VTP(graphics)->drawFragmentedPanel(graphics, &Asset_PanelLarge, 8, 34, 59);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 16, 55, "TOP");
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 16, 63, "0010000");
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 16, 79, "SCORE");
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 16, 87, "0000200");

    VTP(graphics)->drawFragmentedPanel(graphics, &Asset_PanelSmall, 176, 122, 83);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 184, 143, "LEVEL");
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 216, 151, "00");
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 184, 167, "SPEED");
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 208, 175, "MED");
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 184, 191, "VIRUS");
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 216, 199, "03");

    //SDL_SaveBMP(((SDL_Graphics*)graphics)->screen->screenSurface, "a.bmp");
    //ThrowError("debug");
}

static struct GameState_VTABLE _vtable = {
        update, draw, load, unload
};

void InGameState_init(this_p(InGameState), Engine *engine) {
    this->base.engine = engine;
    VT(this->base) = &_vtable;
}
