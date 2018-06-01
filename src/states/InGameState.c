/*
 *  InGameState.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "SDL_Screen.h"
#include "InGameState.h"

/* Sprite prealloc */
#define SPRITE_PREALLOC 50

/* Save bmp for photoshop confrontation */
/* #define SAVE_DEBUG */


/* *************************************************************** */
/* Graphics Elements                                               */
/* *************************************************************** */

static void loadPalette(Graphics* graphics, SinglePlayerGame_Speed speed) {
    /* Load palette according to speed */
    switch (speed) {
        case LOW: graphics->currentPalette = &Asset_DefaultPalette; break;
        case MED: graphics->currentPalette = &Asset_DefaultPalette; break;
        case HI: graphics->currentPalette = &Asset_DefaultPalette; break;
        default: break;
    }
}

static void drawBaseElements(Graphics* graphics) {
    /* Background and Logo */
    VTP(graphics)->drawCheckerboard(graphics, 8, 2, 0);
    VTP(graphics)->drawTexture(graphics, &Asset_Logo, 159, 26, 159 + Asset_Logo.width, 26 + Asset_Logo.height, 0, 0, Asset_Logo.width, Asset_Logo.height);

    /* Boxes */
    VTP(graphics)->drawTexture(graphics, &Asset_MagVirus, 0, 128, 0 + Asset_MagVirus.width, 128 + Asset_MagVirus.height, 0, 0, Asset_MagVirus.width, Asset_MagVirus.height);
    VT(Asset_PanelBottle)->draw(&Asset_PanelBottle, graphics, 88, 40, 128);
    VT(Asset_MarioBox)->draw(&Asset_MarioBox, graphics, 176, 64, 38, 38);
}

static void drawScorePanel(Graphics* graphics, int top, int score) {
    char top_c[8] = {0}, score_c[8] = {0};
    sprintf_s(top_c, 8, "%07d", top);
	sprintf_s(score_c, 8, "%07d", score);

    VT(Asset_PanelLarge)->draw(&Asset_PanelLarge, graphics, 8, 34, 59);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 16, 55, "TOP", 0);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 16, 63, top_c, 0);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 16, 79, "SCORE", 0);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 16, 87, score_c, 0);
}

static void drawLevelPanel(Graphics* graphics, int level, SinglePlayerGame_Speed speed, int virus) {
    char level_c[3] = {0}, virus_c[3] = {0};
	sprintf_s(level_c, 3, "%02d", level);
	sprintf_s(virus_c, 3, "%02d", virus);

    VT(Asset_PanelSmall)->draw(&Asset_PanelSmall, graphics, 176, 122, 83);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 184, 143, "LEVEL", 0);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 216, 151, level_c, 0);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 184, 167, "SPEED", 0);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 184, 191, "VIRUS", 0);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 216, 199, virus_c, 0);

    switch (speed) {
        case LOW: VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 208, 175, "LOW", 0); break;
        case MED: VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 208, 175, "MED", 0); break;
        case HI: VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 208, 175, "HI", 0); break;
        default: break;
    }
}

static void drawEndMessage(this_p(GameState), Graphics* graphics, SinglePlayerGame_State state) {
    if (state == END_WON) {
        VT(Asset_EndBox)->draw(&Asset_EndBox, graphics, 98, 90, 49, 56);
        VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 104, 103, "STAGE", 0);
        VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 112, 111, "CLEAR", 0);
        VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 104, 127, "TRY", 0);
        VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 120, 135, "NEXT", 0);
    } else if (state == END_LOOSE) {
        VT(Asset_EndBox)->draw(&Asset_EndBox, graphics, 98, 98, 49, 40);
        VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 112, 111, "GAME", 0);
        VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 112, 127, "OVER", 0);
    }

    /* 150 ms delay for START */
    if (VTP(this->engine->screen)->getCurrentTime(this->engine->screen) % 300 > 150)
        VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 109, 195, "START", 1);
}


/* Drawing entry point */
static void draw(this_p(GameState)) {
    Graphics *graphics = VTP(this->engine->screen)->getGraphics(this->engine->screen);
    InGameState *state = (InGameState *) this;
    size_t i;

    /* Background */
    drawBaseElements(graphics);

    /* Panels */
    drawScorePanel(graphics, state->logic.top, state->logic.score);
    drawLevelPanel(graphics, state->logic.level, state->logic.speed, state->logic.virus);

    /* End game */
    if (state->logic.state == END_LOOSE || state->logic.state == END_WON)
        drawEndMessage(this, graphics, state->logic.state);

    /* Sprites */
    Vector_foreach(state->sprites, i) {
        Sprite *sprite = VT(state->sprites)->get(&state->sprites, i);
        VTP(sprite)->draw(sprite, this->engine->screen, graphics);
    }

#ifdef SAVE_DEBUG
    SDL_SaveBMP(((SDL_Graphics*)graphics)->screen->screenSurface, "a.bmp");
    ThrowError("debug");
#endif
}


/* *************************************************************** */
/* Game Logic                                                      */
/* *************************************************************** */

static void update(this_p(GameState), double deltaTime) {

}


/* *************************************************************** */
/* Initialization                                                  */
/* *************************************************************** */

static void load(this_p(GameState)) {
    InGameState *state = (InGameState *) this;

    /* Game logic */
    SinglePlayerGame_init(&state->logic);
    VT(state->logic)->newGame(&state->logic);

    /* Sprites vector */
    HeapVector_init(&state->sprites, SPRITE_PREALLOC, sizeof(Sprite));

    /* Mario */
    state->marioSprite = VT(state->sprites)->addEmpty(&state->sprites);
    Sprite_init(state->marioSprite, this->engine->screen, &Asset_Mario, 184, 76, 0);

    /* Large viruses */
    state->virusLargeBlueSprite = VT(state->sprites)->addEmpty(&state->sprites);
    state->virusLargeYellowSprite = VT(state->sprites)->addEmpty(&state->sprites);
    state->virusLargeRedSprite = VT(state->sprites)->addEmpty(&state->sprites);
    Sprite_init(state->virusLargeBlueSprite, this->engine->screen, &Asset_VirusLargeBlue, 31, 136, 0);
    Sprite_init(state->virusLargeYellowSprite, this->engine->screen, &Asset_VirusLargeYellow, 18, 167, 0);
    Sprite_init(state->virusLargeRedSprite, this->engine->screen, &Asset_VirusLargeRed, 46, 164, 0);
}

static void unload(this_p(GameState)) {
    InGameState *state = (InGameState *) this;
    VT(state->sprites)->dispose(&state->sprites);
}

static struct GameState_VTABLE _vtable = {
        update, draw, load, unload
};

void InGameState_init(this_p(InGameState), Engine *engine) {
    this->base.engine = engine;
    VT(this->base) = &_vtable;
}
