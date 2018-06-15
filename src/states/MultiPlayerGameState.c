/*
 *  MultiPlayerGameState.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  View part of a Multi Player Game
 */

#include "MultiPlayerGameState.h"

#include <stdio.h>
#include <math.h>
#ifndef WIN32
#define sprintf_s(buffer, size, stringbuffer, ...) (snprintf(buffer, size, stringbuffer, __VA_ARGS__))
#endif

/* Prealloc */
#define TIMELINE_PREALLOC 64

/* Save bmp screenshoot */
/*#define SAVE_DEBUG
#include "SDL_Screen.h"*/


/* *************************************************************** */
/* Graphics Elements                                               */
/* *************************************************************** */

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
        case SinglePlayerSpeed_Low: VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 208, 175, "LOW", 0); break;
        case SinglePlayerSpeed_Med: VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 208, 175, "MED", 0); break;
        case SinglePlayerSpeed_Hi: VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 208, 175, "HI", 0); break;
        default: break;
    }
}

static void drawEndMessage(this_p(GameState), Graphics* graphics, SinglePlayerGame_State state) {
    if (state == SinglePlayerState_EndWon) {
        VT(Asset_EndBox)->draw(&Asset_EndBox, graphics, 98, 90, 49, 56);
        VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 104, 103, "STAGE", 0);
        VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 112, 111, "CLEAR", 0);
        VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 104, 127, "TRY", 0);
        VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 120, 135, "NEXT", 0);
    } else if (state == SinglePlayerState_EndLost) {
        VT(Asset_EndBox)->draw(&Asset_EndBox, graphics, 98, 98, 49, 40);
        VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 112, 111, "GAME", 0);
        VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 112, 127, "OVER", 0);
    }

    /* 150 ms delay for START */
    if (VTP(this->engine->screen)->getCurrentTime(this->engine->screen) % 300 > 150)
        VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 109, 195, "START", 1);
}

static void drawGameBoard(this_p(MultiPlayerGameState), Screen* screen) {
    uint32_t x, y;

    for (x = 0; x < this->logic.board->board.width; x++) {
        for (y = 0; y < this->logic.board->board.height; y++) {
            GameBoardElement *element = VT(this->logic.board->board)->get2D(&this->logic.board->board, y, x);

            switch (element->type) {
                case GameBoardElement_Virus:
                    drawVirus(screen, y, x, element->color);
                    break;
                case GameBoardElement_Pill:
                    drawPill(screen, y, x, element->color, checkPillNeighborhoods(&this->logic.board->board, x, y));
                    break;
                default:
                    break;
            }
        }
    }
}

static void drawNextPill(this_p(MultiPlayerGameState), Graphics *graphics) {
    Texture *pillTextureL, *pillTextureR;

    pillTextureL = getPillTexture(this->logic.nextPillColorL, Left);
    pillTextureR = getPillTexture(this->logic.nextPillColorR, Right);

    /*VTP(graphics)->drawTexture(graphics, pillTextureL,
                               this->nextPillLX, this->nextPillLY,
                               this->nextPillLX + pillTextureL->width, this->nextPillLY + pillTextureL->height,
                               0, 0, pillTextureL->width, pillTextureL->height);
    VTP(graphics)->drawTexture(graphics, pillTextureR,
                               this->nextPillRX, this->nextPillRY,
                               this->nextPillRX + pillTextureR->width, this->nextPillRY + pillTextureR->height,
                               0, 0, pillTextureR->width, pillTextureR->height);*/
}


/* Drawing entry point */
static void draw(this_p(GameState)) {
    Graphics *graphics = VTP(this->engine->screen)->getGraphics(this->engine->screen);
    MultiPlayerGameState *state = (MultiPlayerGameState *) this;

    /* Background */
    drawBaseElements(graphics);

    /* Panels */
    drawScorePanel(graphics, state->logic.top, state->logic.score);
    drawLevelPanel(graphics, state->logic.level, state->logic.speed, state->logic.virusCount);

    /* Board */
    drawGameBoard(state, this->engine->screen);

    /* End game */
    if (state->logic.state == SinglePlayerState_EndLost || state->logic.state == SinglePlayerState_EndWon)
        drawEndMessage(this, graphics, state->logic.state);

    /* Sprites */
    VT(state->marioSprite)->draw(&state->marioSprite, this->engine->screen, graphics);
    VT(state->virusLargeRedSprite)->draw(&state->virusLargeRedSprite, this->engine->screen, graphics);
    VT(state->virusLargeYellowSprite)->draw(&state->virusLargeYellowSprite, this->engine->screen, graphics);
    VT(state->virusLargeBlueSprite)->draw(&state->virusLargeBlueSprite, this->engine->screen, graphics);

    /* Current Pill */
    drawNextPill(state, graphics);

#ifdef SAVE_DEBUG
    SDL_SaveBMP(((SDL_Graphics*)graphics)->screen->screenSurface, "a.bmp");
    ThrowError("debug");
#endif
}


/* *************************************************************** */
/* Game Logic                                                      */
/* *************************************************************** */

static void updateAnimations(this_p(MultiPlayerGameState)) {
    if (this->logic.state != this->lastLogicState) {
        switch (this->logic.state) {
            case SinglePlayerState_WaitingForPill:
                break;
            case SinglePlayerState_Moving:
                break;
            case SinglePlayerState_NoControl:
                break;
            case SinglePlayerState_EndWon:
                break;
            case SinglePlayerState_EndLost:
                VT(this->virusLargeBlueSprite)->setAnimation(&this->virusLargeBlueSprite, this->base.engine->screen, 1);
                VT(this->virusLargeRedSprite)->setAnimation(&this->virusLargeRedSprite, this->base.engine->screen, 1);
                VT(this->virusLargeYellowSprite)->setAnimation(&this->virusLargeYellowSprite, this->base.engine->screen, 1);
                VT(this->marioSprite)->setAnimation(&this->marioSprite, this->base.engine->screen, 2);
                break;
            default:
                break;
        }
        this->lastLogicState = this->logic.state;
    }
}

static void pillLaunchAnim(this_p(MultiPlayerGameState), uint32_t currentTime) {

}

static PillDirection getDirectionFromKeyboard(this_p(MultiPlayerGameState)) {
    InputState *inputState = VTP(this->base.engine->inputDevice)->getInputState(this->base.engine->inputDevice);
    if (inputState->leftButton) return PillDirection_Left;
    else if (inputState->rightButton) return PillDirection_Right;
    else if (inputState->downButton) return PillDirection_Down;
    else return PillDirection_Nothing;
}

static bool update(this_p(GameState)) {
    MultiPlayerGameState *state = (MultiPlayerGameState *)this;
    uint32_t currentTime = VTP(this->engine->screen)->getCurrentTime(this->engine->screen);

    /* Update controller */
    VT(state->logic)->update(&state->logic, this->engine, getDirectionFromKeyboard(state));
    VTP(this->engine->inputDevice)->reset(this->engine->inputDevice);

    /* Timeline */
    VT(state->timeline)->update(&state->timeline,currentTime);

    /* Change view */
    updateAnimations(state);
    pillLaunchAnim(state, currentTime);

    return true;
}


/* *************************************************************** */
/* Initialization                                                  */
/* *************************************************************** */

static void startGame_AfterAnim(this_p(MultiPlayerGameState)) {
}

static void startGame(this_p(MultiPlayerGameState)) {
    uint32_t startTime = VTP(this->base.engine->screen)->getCurrentTime(this->base.engine->screen);
    VT(this->marioSprite)->setAnimation(&this->marioSprite, this->base.engine->screen, 1);
    VT(this->timeline)->addEvent(&this->timeline, (void (*)(void *)) startGame_AfterAnim, startTime + 300, this);
}

static void load(this_p(GameState)) {
    MultiPlayerGameState *state = (MultiPlayerGameState *) this;
    uint32_t startTime = VTP(this->engine->screen)->getCurrentTime(this->engine->screen);

    Timeline_init(&state->timeline, TIMELINE_PREALLOC);

    /* Sprites */
    Sprite_init(&state->marioSprite, this->engine->screen, &Asset_Mario, 184, 76, 4);
    Sprite_init(&state->virusLargeBlueSprite, this->engine->screen, &Asset_VirusLargeBlue, 31, 136, 0);
    Sprite_init(&state->virusLargeYellowSprite, this->engine->screen, &Asset_VirusLargeYellow, 18, 167, 0);
    Sprite_init(&state->virusLargeRedSprite, this->engine->screen, &Asset_VirusLargeRed, 46, 164, 0);

    VT(state->timeline)->addEvent(&state->timeline, (void (*)(void *)) startGame, startTime + 1000, this);
}

static void unload(this_p(GameState)) {
    MultiPlayerGameState *state = (MultiPlayerGameState *) this;
    VT(state->timeline)->dispose(&state->timeline);
}

static struct GameState_VTABLE _vtable = {
        update, draw
};

void MultiPlayerGameState_init(this_p(MultiPlayerGameState), Engine *engine, int top, int level, int virus, SinglePlayerGame_Speed speed) {
    this->base.engine = engine;
    VT(this->base) = &_vtable;

    //SinglePlayerGame_init(&this->logic, engine, top, level, virus, speed);
    this->lastLogicState = SinglePlayerState_Nothing;
}
