/*
 *  SinglePlayerGameState.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *  View part of a Single Player Game
 */

#include "SinglePlayerGameState.h"

#include <stdio.h>
#include <math.h>
#ifndef WIN32
#define sprintf_s(buffer, buffer_size, stringbuffer, ...) (sprintf(buffer, stringbuffer, __VA_ARGS__))
#endif

/* Prealloc */
#define TIMELINE_PREALLOC 64

/* Save bmp screenshoot */
/*#define SAVE_DEBUG
#include "SDL_Screen.h"*/


/* *************************************************************** */
/* Graphics Elements                                               */
/* *************************************************************** */

static void loadPalette(Graphics* graphics, SinglePlayerGame_Speed speed) {
    /* Load palette according to speed */
    switch (speed) {
        case SinglePlayerSpeed_Low: graphics->currentPalette = &Asset_MedPalette; break;
        case SinglePlayerSpeed_Med: graphics->currentPalette = &Asset_MedPalette; break;
        case SinglePlayerSpeed_Hi: graphics->currentPalette = &Asset_MedPalette; break;
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

static void drawVirus(Screen* screen, uint32_t row, uint32_t col, GameBoardElementColor color) {
    Graphics *graphics = VTP(screen)->getGraphics(screen);
    uint32_t x, y, frame;
    Sprite virusSprite = { 0 };

    x = (uint32_t) (95 + col * (Asset_VirusRed.spriteWidth - 1));
    y = (uint32_t) (199 - row * (Asset_VirusRed.spriteHeight - 1));
    frame = (uint32_t) ((int)(VTP(screen)->getCurrentTime(screen) * 0.005) % 2);

    switch (color) {
        case GameBoardElement_Red:
            Sprite_init(&virusSprite, screen, &Asset_VirusRed, x, y, 0);
            frame += 4;
            break;
        case GameBoardElement_Blue:
            Sprite_init(&virusSprite, screen, &Asset_VirusBlue, x, y, 0);
            break;
        case GameBoardElement_Yellow:
            Sprite_init(&virusSprite, screen, &Asset_VirusYellow, x, y, 0);
            frame += 2;
            break;
    }

    VT(virusSprite)->drawFrame(&virusSprite, screen, graphics, frame);
}

static void drawPill(Screen* screen, uint32_t row, uint32_t col, GameBoardElementColor color) {
    Graphics *graphics = VTP(screen)->getGraphics(screen);
    uint32_t x, y;
    Texture *pillTexture;

    x = (uint32_t) (96 + col * (Asset_PillRed.width + 1));
    y = (uint32_t) (200 - row * (Asset_PillRed.height + 1));

    switch (color) {
        case GameBoardElement_Red:
            pillTexture = &Asset_PillRed_A;
            break;
        case GameBoardElement_Blue:
            pillTexture = &Asset_PillBlue_A;
            break;
        case GameBoardElement_Yellow:
            pillTexture = &Asset_PillYellow_A;
            break;
        default:
            return;
    };

    VTP(graphics)->drawTexture(graphics, pillTexture,
                               x, y, x + pillTexture->width, y + pillTexture->height,
                               0, 0, pillTexture->width, pillTexture->height);
}

static void drawGameBoard(this_p(SinglePlayerGameState), Screen* screen) {
    uint32_t x, y;

    for (x = 0; x < this->logic.board.width; x++) {
        for (y = 0; y < this->logic.board.height; y++) {
            GameBoardElement *element = VT(this->logic.board)->get2D(&this->logic.board, y, x);
            if (element->type == GameBoardElement_Virus) {
                drawVirus(screen, y, x, element->color);
            } else if (element->type == GameBoardElement_Pill) {
                drawPill(screen, y, x, element->color);
            }
        }
    }
}


/* Drawing entry point */
static void draw(this_p(GameState)) {
    Graphics *graphics = VTP(this->engine->screen)->getGraphics(this->engine->screen);
    SinglePlayerGameState *state = (SinglePlayerGameState *) this;

    /* Background */
    loadPalette(graphics, state->logic.speed);
    drawBaseElements(graphics);

    /* Panels */
    drawScorePanel(graphics, state->logic.top, state->logic.score);
    drawLevelPanel(graphics, state->logic.level, state->logic.speed, state->logic.virus);

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

#ifdef SAVE_DEBUG
    SDL_SaveBMP(((SDL_Graphics*)graphics)->screen->screenSurface, "a.bmp");
    ThrowError("debug");
#endif
}


/* *************************************************************** */
/* Game Logic                                                      */
/* *************************************************************** */

static void updateAnimations(this_p(SinglePlayerGameState)) {
	if (this->logic.state != this->lastLogicState) {
		switch (this->logic.state) {
			case SinglePlayerState_Playing:
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

static void rotateVirusLarge(this_p(SinglePlayerGameState), uint32_t time) {
	float centerX = 31.78f;
	float centerY = 155.13f;
	float radius = 18.13f;
	float angle = -time * 0.0002f;

	this->virusLargeBlueSprite.x = (uint32_t)(centerX + 0.9f * radius * cosf(angle - 1.57f));
	this->virusLargeBlueSprite.y = (uint32_t)(centerY + 1.1f * radius * sinf(angle - 1.57f));
	this->virusLargeYellowSprite.x = (uint32_t)(centerX + 0.9f * radius * cosf(angle + 2.353f));
	this->virusLargeYellowSprite.y = (uint32_t)(centerY + 1.1f * radius * sinf(angle + 2.353f));
	this->virusLargeRedSprite.x = (uint32_t)(centerX + 0.9f * radius * cosf(angle + 0.575f));
	this->virusLargeRedSprite.y = (uint32_t)(centerY + 1.1f * radius * sinf(angle + 0.575f));
}

static void update(this_p(GameState)) {
	SinglePlayerGameState *state = (SinglePlayerGameState *)this;

	/* Update controller */
	VT(state->logic)->update(&state->logic, VTP(this->engine->screen)->getCurrentTime(this->engine->screen));

    /* Timeline */
    VT(state->timeline)->update(&state->timeline, VTP(this->engine->screen)->getCurrentTime(this->engine->screen));

	/* Change view */
	updateAnimations(state);
	rotateVirusLarge(state, VTP(this->engine->screen)->getCurrentTime(this->engine->screen));
}


/* *************************************************************** */
/* Initialization                                                  */
/* *************************************************************** */

static void load(this_p(GameState)) {
    SinglePlayerGameState *state = (SinglePlayerGameState *) this;

    Timeline_init(&state->timeline, TIMELINE_PREALLOC);

    /* Sprites */
    Sprite_init(&state->marioSprite, this->engine->screen, &Asset_Mario, 184, 76, 0);
    Sprite_init(&state->virusLargeBlueSprite, this->engine->screen, &Asset_VirusLargeBlue, 31, 136, 0);
    Sprite_init(&state->virusLargeYellowSprite, this->engine->screen, &Asset_VirusLargeYellow, 18, 167, 0);
	Sprite_init(&state->virusLargeRedSprite, this->engine->screen, &Asset_VirusLargeRed, 46, 164, 0);
}

static void unload(this_p(GameState)) {
    SinglePlayerGameState *state = (SinglePlayerGameState *) this;
    VT(state->timeline)->dispose(&state->timeline);
}

static struct GameState_VTABLE _vtable = {
        update, draw, load, unload
};

void SinglePlayerGameState_init(this_p(SinglePlayerGameState), Engine *engine, int top, int level, int virus, SinglePlayerGame_Speed speed) {
    this->base.engine = engine;
    VT(this->base) = &_vtable;

    SinglePlayerGame_init(&this->logic, top, level, virus, speed);
    this->lastLogicState = SinglePlayerState_Nothing;
}
