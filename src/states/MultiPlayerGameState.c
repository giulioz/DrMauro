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

static void unload(this_p(MultiPlayerGameState)) {
	VT(this->timeline)->dispose(&this->timeline);
}


/* *************************************************************** */
/* Graphics Elements                                               */
/* *************************************************************** */

static void drawBaseElements(Graphics* graphics) {
    /* Background and Logo */
    VTP(graphics)->drawCheckerboard(graphics, 8, 2, 0);
}

static void drawScorePanel(Graphics* graphics, size_t top, size_t score) {
    char top_c[8] = {0}, score_c[8] = {0};
    sprintf_s(top_c, 8, "%07d", top);
    sprintf_s(score_c, 8, "%07d", score);

    VT(Asset_PanelLarge)->draw(&Asset_PanelLarge, graphics, 8, 34, 59);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 16, 55, "TOP", 0);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 16, 63, top_c, 0);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 16, 79, "SCORE", 0);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 16, 87, score_c, 0);
}

static void drawLevelPanel(Graphics* graphics, size_t level, size_t speed, size_t virus) {
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

static void drawGameBoard(this_p(MultiPlayerGameState), Screen* screen, SinglePlayerGame *logic, uint32_t sx) {
    uint32_t x, y;

    for (x = 0; x < logic->board->board.width; x++) {
        for (y = 0; y < logic->board->board.height; y++) {
            GameBoardElement *element = VT(logic->board->board)->get2D(&logic->board->board, y, x);

            switch (element->type) {
                case GameBoardElement_Virus:
                    drawVirus(screen, y, x, element->color);
                    break;
                case GameBoardElement_Pill:
                    drawPill(screen, y, x, element->color, checkPillNeighborhoods(&logic->board->board, x, y));
                    break;
                default:
                    break;
            }
        }
    }
}

static void drawNextPill(this_p(MultiPlayerGameState), Graphics *graphics, SinglePlayerGame *logic, uint32_t x) {
    Texture *pillTextureL, *pillTextureR;

    pillTextureL = getPillTexture(logic->nextPillColorL, Left);
    pillTextureR = getPillTexture(logic->nextPillColorR, Right);

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
    //drawScorePanel(graphics, state->logic.top, state->logic.score);
    //drawLevelPanel(graphics, state->logic.level, state->logic.speedProvider->speed, state->logic.virusCount);

    /* Boards */
    drawGameBoard(state, this->engine->screen, state->logic1, 0);
	drawGameBoard(state, this->engine->screen, state->logic2, 100);

    /* End game */
    //if (state->logic.state == SinglePlayerState_EndLost || state->logic.state == SinglePlayerState_EndWon)
        //drawEndMessage(this, graphics, state->logic.state);

    /* Current Pill */
    //drawNextPill(state, graphics);

#ifdef SAVE_DEBUG
    SDL_SaveBMP(((SDL_Graphics*)graphics)->screen->screenSurface, "a.bmp");
    ThrowError("debug");
#endif
}


/* *************************************************************** */
/* Game Logic                                                      */
/* *************************************************************** */

static bool update(this_p(GameState)) {
    MultiPlayerGameState *state = (MultiPlayerGameState *)this;
    uint32_t currentTime = VTP(this->engine->screen)->getCurrentTime(this->engine->screen);
	InputState *inputState = VTP(state->base.engine->inputDevice)->getInputState(state->base.engine->inputDevice);

	/* Allow exit */
	if ((state->logic1->state == SinglePlayerState_EndLost || state->logic1->state == SinglePlayerState_EndWon
		|| state->logic2->state == SinglePlayerState_EndLost || state->logic2->state == SinglePlayerState_EndWon)
		&& inputState->enterButton) {
		unload(state);
		return false;
	}

	if (!(state->logic1->state == SinglePlayerState_EndLost || state->logic1->state == SinglePlayerState_EndWon
		|| state->logic2->state == SinglePlayerState_EndLost || state->logic2->state == SinglePlayerState_EndWon)) {
		VTP(state->logic1)->update(state->logic1, this->engine, inputState->direction1);
		VTP(state->logic2)->update(state->logic2, this->engine, inputState->direction2);
	}

    VTP(this->engine->inputDevice)->reset(this->engine->inputDevice);

    /* Timeline */
    VT(state->timeline)->update(&state->timeline,currentTime);

    return true;
}


/* *************************************************************** */
/* Initialization                                                  */
/* *************************************************************** */

static struct GameState_VTABLE _vtable = {
        update, draw
};

void MultiPlayerGameState_init(this_p(MultiPlayerGameState), Engine *engine, SinglePlayerGame *logic1, SinglePlayerGame *logic2) {
    this->base.engine = engine;
    VT(this->base) = &_vtable;

	this->logic1 = logic1;
	this->logic2 = logic2;

	Timeline_init(&this->timeline, TIMELINE_PREALLOC);

    this->lastLogicState1 = SinglePlayerState_Nothing;
	this->lastLogicState2 = SinglePlayerState_Nothing;
}
