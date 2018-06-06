/*
 *  MainManuGameState.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "MainMenuGameState.h"

/* Save bmp screenshoot */
//#define SAVE_DEBUG
#include "SDL_Screen.h"


/* *************************************************************** */
/* Graphics Elements                                               */
/* *************************************************************** */

/* Drawing entry point */
static void draw(this_p(GameState)) {
    Graphics *graphics = VTP(this->engine->screen)->getGraphics(this->engine->screen);
    MainMenuGameState *state = (MainMenuGameState *) this;

    /* Background */
    graphics->currentPalette = &Asset_MenuPalette;
    VTP(graphics)->drawCheckerboard(graphics, 8, 2, 0);
    VT(Asset_MainMenuBox)->draw(&Asset_MainMenuBox, graphics, 33, 160, 126, 8);
    VTP(graphics)->drawTexture(graphics, &Asset_BigPill, 16, 40, 16 + Asset_BigPill.width, 40 + Asset_BigPill.height, 0, 0, Asset_BigPill.width, Asset_BigPill.height);

    /* Strings */
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 80, 167, "1 PLAYER GAME", 1);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 80, 183, "2 PLAYER GAME", 1);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 80, 200, "Q", 1);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 80, 199, "  2018 CT0304", 1);

    /* Sprites */
    VT(state->marioSprite)->draw(&state->marioSprite, this->engine->screen, graphics);
    VT(state->virusSprite)->draw(&state->virusSprite, this->engine->screen, graphics);
    VT(state->titleSprite)->draw(&state->titleSprite, this->engine->screen, graphics);
    state->selectorSprite.y = (uint32_t) (169 + state->selectedMenuEntry * 16);
    VT(state->selectorSprite)->draw(&state->selectorSprite, this->engine->screen, graphics);

#ifdef SAVE_DEBUG
    SDL_SaveBMP(((SDL_Graphics*)graphics)->screen->screenSurface, "a.bmp");
    ThrowError("debug");
#endif
}


/* *************************************************************** */
/* Game Logic                                                      */
/* *************************************************************** */

static void update(this_p(GameState)) {
    MainMenuGameState *state = (MainMenuGameState *)this;
    InputState *inputState = VTP(this->engine->inputDevice1)->getInputState(this->engine->inputDevice1);

    if (inputState->downButton) {
        state->selectedMenuEntry = 1;
    } else if (inputState->upButton) {
        state->selectedMenuEntry = 0;
    }

    if (inputState->enterButton) {
        if (state->selectedMenuEntry == 0) {
            DifficultySelectionGameState_init(&state->difficultySelectionGameState, this->engine, false);
        } else if (state->selectedMenuEntry == 1) {
            DifficultySelectionGameState_init(&state->difficultySelectionGameState, this->engine, true);
        }
        VTP(this->engine)->loadState(this->engine, (GameState *) &state->difficultySelectionGameState);
    }
}


/* *************************************************************** */
/* Initialization                                                  */
/* *************************************************************** */

static void load(this_p(GameState)) {
    MainMenuGameState *state = (MainMenuGameState *) this;
    state->selectedMenuEntry = 0;

    /* Sprites */
    Sprite_init(&state->marioSprite, this->engine->screen, &Asset_Mario, 37, 168, 3);
    Sprite_init(&state->virusSprite, this->engine->screen, &Asset_VirusLargeBlue, 192, 177, 0);
    Sprite_init(&state->titleSprite, this->engine->screen, &Asset_BigTitle, 34, 63, 0);
    Sprite_init(&state->selectorSprite, this->engine->screen, &Asset_Heart, 69, 169, 0);
}

static void unload(this_p(GameState)) {

}

static struct GameState_VTABLE _vtable = {
        update, draw, load, unload
};

void MainMenuGameState_init(this_p(MainMenuGameState), Engine *engine) {
    this->base.engine = engine;
    VT(this->base) = &_vtable;
}
