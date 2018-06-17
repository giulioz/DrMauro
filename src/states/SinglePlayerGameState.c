/*
 *  SinglePlayerGameState.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  View part of a Single Player Game
 */

#include "SinglePlayerGameState.h"

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

static void unload(this_p(SinglePlayerGameState)) {
    VT(this->timeline)->dispose(&this->timeline);
}


/* *************************************************************** */
/* Graphics Elements                                               */
/* *************************************************************** */

static void loadPalette(Graphics* graphics, size_t speed) {
    /* Load palette according to speed */
    switch (speed) {
        case 0: graphics->currentPalette = &Asset_LowPalette; break;
        case 1: graphics->currentPalette = &Asset_MedPalette; break;
        case 2: graphics->currentPalette = &Asset_HiPalette; break;
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

static void drawScorePanel(Graphics* graphics, size_t top, size_t score) {
    char top_c[8] = {0}, score_c[8] = {0};
    sprintf_s(top_c, 8, "%07d", (int)top);
	sprintf_s(score_c, 8, "%07d", (int)score);

    VT(Asset_PanelLarge)->draw(&Asset_PanelLarge, graphics, 8, 34, 59);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 16, 55, "TOP", 0);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 16, 63, top_c, 0);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 16, 79, "SCORE", 0);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 16, 87, score_c, 0);
}

static void drawLevelPanel(Graphics* graphics, size_t level, size_t speed, size_t virus) {
    char level_c[3] = {0}, virus_c[3] = {0};
	sprintf_s(level_c, 3, "%02d", (int)level);
	sprintf_s(virus_c, 3, "%02d", (int)virus);

    VT(Asset_PanelSmall)->draw(&Asset_PanelSmall, graphics, 176, 122, 83);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 184, 143, "LEVEL", 0);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 216, 151, level_c, 0);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 184, 167, "SPEED", 0);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 184, 191, "VIRUS", 0);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 216, 199, virus_c, 0);

    switch (speed) {
        case 0: VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 208, 175, "LOW", 0); break;
        case 1: VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 208, 175, "MED", 0); break;
        case 2: VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 208, 175, "HI", 0); break;
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

static void drawGameBoard(this_p(SinglePlayerGameState), Screen* screen) {
    uint32_t x, y;

    for (x = 0; x < this->logic->board->board.width; x++) {
        for (y = 0; y < this->logic->board->board.height; y++) {
            GameBoardElement *element = VT(this->logic->board->board)->get2D(&this->logic->board->board, y, x);

            switch (element->type) {
                case GameBoardElement_Virus:
                    drawVirus(screen, y, x, element->color);
                    break;
                case GameBoardElement_Pill:
                    drawPill(screen, y, x, element->color, checkPillNeighborhoods(&this->logic->board->board, x, y));
                    break;
                default:
                    break;
            }
        }
    }
}

static void drawNextPill(this_p(SinglePlayerGameState), Graphics *graphics) {
    Texture *pillTextureL, *pillTextureR;

    pillTextureL = getPillTexture(this->logic->nextPillColorL, Left);
    pillTextureR = getPillTexture(this->logic->nextPillColorR, Right);

    VTP(graphics)->drawTexture(graphics, pillTextureL,
                               this->nextPillLX, this->nextPillLY,
                               this->nextPillLX + pillTextureL->width, this->nextPillLY + pillTextureL->height,
                               0, 0, pillTextureL->width, pillTextureL->height);
    VTP(graphics)->drawTexture(graphics, pillTextureR,
                               this->nextPillRX, this->nextPillRY,
                               this->nextPillRX + pillTextureR->width, this->nextPillRY + pillTextureR->height,
                               0, 0, pillTextureR->width, pillTextureR->height);
}


/* Drawing entry point */
static void draw(this_p(GameState)) {
    Graphics *graphics = VTP(this->engine->screen)->getGraphics(this->engine->screen);
    SinglePlayerGameState *state = (SinglePlayerGameState *) this;

    loadPalette(graphics, state->logic->speedProvider->speed);
    drawBaseElements(graphics);

    /* Panels */
    drawScorePanel(graphics, state->logic->top, state->logic->score);
    drawLevelPanel(graphics, state->logic->level, state->logic->speedProvider->speed, state->logic->virusCount);

	if (state->logic->state != SinglePlayerState_EndLost && state->logic->state != SinglePlayerState_EndWon)
		drawGameBoard(state, this->engine->screen);
	else
        drawEndMessage(this, graphics, state->logic->state);

    /* Sprites */
    VT(state->marioSprite)->draw(&state->marioSprite, this->engine->screen, graphics);
    VT(state->virusLargeRedSprite)->draw(&state->virusLargeRedSprite, this->engine->screen, graphics);
    VT(state->virusLargeYellowSprite)->draw(&state->virusLargeYellowSprite, this->engine->screen, graphics);
    VT(state->virusLargeBlueSprite)->draw(&state->virusLargeBlueSprite, this->engine->screen, graphics);

    /* Current Pill */
    if (state->nextPillVisible)
        drawNextPill(state, graphics);

#ifdef SAVE_DEBUG
    SDL_SaveBMP(((SDL_Graphics*)graphics)->screen->screenSurface, "a.bmp");
    ThrowError("debug");
#endif
}


/* *************************************************************** */
/* Game Logic                                                      */
/* *************************************************************** */

static void rotateVirusLarge(this_p(SinglePlayerGameState), uint32_t time) {
	if (this->rotationEnabled) {
        float centerX = 31.78f;
        float centerY = 155.13f;
        float radius = 18.13f;
        float angle = -this->rotTime * 0.0002f;

        this->virusLargeBlueSprite.x = (uint32_t)(centerX + 0.9f * radius * cosf(angle - 1.57f));
        this->virusLargeBlueSprite.y = (uint32_t)(centerY + 1.1f * radius * sinf(angle - 1.57f));
        this->virusLargeYellowSprite.x = (uint32_t)(centerX + 0.9f * radius * cosf(angle + 2.353f));
        this->virusLargeYellowSprite.y = (uint32_t)(centerY + 1.1f * radius * sinf(angle + 2.353f));
        this->virusLargeRedSprite.x = (uint32_t)(centerX + 0.9f * radius * cosf(angle + 0.575f));
        this->virusLargeRedSprite.y = (uint32_t)(centerY + 1.1f * radius * sinf(angle + 0.575f));

        this->rotTime += (time - this->lastRot);
    }
    this->lastRot = time;
}

static void endVirusAnimationRed(this_p(SinglePlayerGameState)) {
    VT(this->virusLargeRedSprite)->setAnimation(&this->virusLargeRedSprite, this->base.engine->screen, 0);
    if (!isVirusPresent(this->logic->board, GameBoardElement_Red))
        this->virusLargeRedSprite.visible = false;
    this->rotationEnabled = true;
}

static void endVirusAnimationYellow(this_p(SinglePlayerGameState)) {
    VT(this->virusLargeYellowSprite)->setAnimation(&this->virusLargeYellowSprite, this->base.engine->screen, 0);
    if (!isVirusPresent(this->logic->board, GameBoardElement_Yellow))
        this->virusLargeYellowSprite.visible = false;
    this->rotationEnabled = true;
}

static void endVirusAnimationBlue(this_p(SinglePlayerGameState)) {
    VT(this->virusLargeBlueSprite)->setAnimation(&this->virusLargeBlueSprite, this->base.engine->screen, 0);
    if (!isVirusPresent(this->logic->board, GameBoardElement_Blue))
        this->virusLargeBlueSprite.visible = false;
    this->rotationEnabled = true;
}

static void newPill_AfterAnimMario(this_p(SinglePlayerGameState)) {
	this->nextPillVisible = true;
	VT(this->marioSprite)->setAnimation(&this->marioSprite, this->base.engine->screen, 4);
}

static void newPill_AfterAnim(this_p(SinglePlayerGameState)) {
	uint32_t startTime = VTP(this->base.engine->screen)->getCurrentTime(this->base.engine->screen);
	this->logic->state = SinglePlayerState_Ready;
	VT(this->timeline)->addEvent(&this->timeline, (void(*)(void *)) newPill_AfterAnimMario,
		startTime + 200, this);
}

static void newPill(this_p(SinglePlayerGameState)) {
	uint32_t startTime = VTP(this->base.engine->screen)->getCurrentTime(this->base.engine->screen);
    this->nextPillVisible = false;
	VT(this->marioSprite)->setAnimation(&this->marioSprite, this->base.engine->screen, 1);
	VT(this->timeline)->addEvent(&this->timeline, (void(*)(void *)) newPill_AfterAnim,
                                 startTime + this->logic->speedProvider->nextPillDelay, this);
}

static void updateAnimations(this_p(SinglePlayerGameState)) {
    uint32_t currentTime = VTP(this->base.engine->screen)->getCurrentTime(this->base.engine->screen);

    if (this->logic->lastVirusRemovedColor != GameBoardElement_NoColor) {
        switch (this->logic->lastVirusRemovedColor) {
            case GameBoardElement_Red:
                VT(this->virusLargeRedSprite)->setAnimation(&this->virusLargeRedSprite, this->base.engine->screen, 2);
                VT(this->timeline)->addEvent(&this->timeline,
                                             (void (*)(void *)) endVirusAnimationRed, currentTime + 3000, this);
                break;
            case GameBoardElement_Blue:
                VT(this->virusLargeBlueSprite)->setAnimation(&this->virusLargeBlueSprite, this->base.engine->screen, 2);
                VT(this->timeline)->addEvent(&this->timeline,
                                             (void (*)(void *)) endVirusAnimationBlue, currentTime + 3000, this);
                break;
            case GameBoardElement_Yellow:
                VT(this->virusLargeYellowSprite)->setAnimation(&this->virusLargeYellowSprite, this->base.engine->screen, 2);
                VT(this->timeline)->addEvent(&this->timeline,
                                             (void (*)(void *)) endVirusAnimationYellow, currentTime + 3000, this);
                break;
            default:
                break;
        }
        this->rotationEnabled = false;
        this->logic->lastVirusRemovedColor = GameBoardElement_NoColor;
    }

    if (this->logic->state != this->lastLogicState) {
        switch (this->logic->state) {
            case SinglePlayerState_WaitingForPill:
                if (this->lastLogicState == SinglePlayerState_FillingBoard)
                    VT(this->timeline)->addEvent(&this->timeline, (void (*)(void *)) newPill,
                                             currentTime + this->logic->speedProvider->firstPillTimeout, this);
                else
                    VT(this->timeline)->addEvent(&this->timeline, (void (*)(void *)) newPill,
                                                 currentTime + this->logic->speedProvider->nextPillDelay, this);
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
        this->lastLogicState = this->logic->state;
    }
}


static bool update(this_p(GameState)) {
	SinglePlayerGameState *state = (SinglePlayerGameState *)this;
    uint32_t currentTime = VTP(this->engine->screen)->getCurrentTime(this->engine->screen);
    InputState *inputState = VTP(state->base.engine->inputDevice)->getInputState(state->base.engine->inputDevice);

    /* Allow exit */
    if ((state->logic->state == SinglePlayerState_EndLost || state->logic->state == SinglePlayerState_EndWon)
            && inputState->enterButton) {
        unload(state);
        return false;
    }

	/* Update controller */
	VTP(state->logic)->update(state->logic, this->engine, getDirectionFromKeyboard(this->engine));
    VTP(this->engine->inputDevice)->reset(this->engine->inputDevice);

    /* Timeline */
    VT(state->timeline)->update(&state->timeline,currentTime);

	/* Change view */
	updateAnimations(state);
	if (state->logic->state < SinglePlayerState_EndWon)
        rotateVirusLarge(state, currentTime);

    return true;
}


/* *************************************************************** */
/* Initialization                                                  */
/* *************************************************************** */

static struct GameState_VTABLE _vtable = {
        update, draw
};

void SinglePlayerGameState_init(this_p(SinglePlayerGameState), Engine *engine, SinglePlayerGame *logic) {
    uint32_t startTime = VTP(engine->screen)->getCurrentTime(engine->screen);

    this->base.engine = engine;
    VT(this->base) = &_vtable;

    this->logic = logic;
    this->lastLogicState = SinglePlayerState_Nothing;

    Timeline_init(&this->timeline, TIMELINE_PREALLOC);

    /* Sprites */
    Sprite_init(&this->marioSprite, this->base.engine->screen, &Asset_Mario, 184, 76, 4);
    Sprite_init(&this->virusLargeBlueSprite, this->base.engine->screen, &Asset_VirusLargeBlue, 31, 136, 0);
    Sprite_init(&this->virusLargeYellowSprite, this->base.engine->screen, &Asset_VirusLargeYellow, 18, 167, 0);
    Sprite_init(&this->virusLargeRedSprite, this->base.engine->screen, &Asset_VirusLargeRed, 46, 164, 0);

    /* Next pill */
    this->nextPillVisible = true;
    this->nextPillLX = 190;
    this->nextPillLY = 70;
    this->nextPillRX = 198;
    this->nextPillRY = 70;

    this->lastRemoved = GameBoardElement_NoColor;
    this->rotationEnabled = true;
    this->rotTime = startTime;
    this->lastRot = startTime;
}
