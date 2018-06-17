/*
 *  DifficultySelectionGameState.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "DifficultySelectionGameState.h"

#include <stdio.h>
#include <string.h>
#include <states/DifficultySelectionGameState.h>

#ifndef WIN32
#define sprintf_s(buffer, size, stringbuffer, ...) (snprintf(buffer, size, stringbuffer, __VA_ARGS__))
#endif

/* Save bmp screenshoot */
/*#define SAVE_DEBUG
#include "SDL_Screen.h"*/


/* *************************************************************** */
/* Graphics Elements                                               */
/* *************************************************************** */

static void drawTitle(Graphics* graphics, int playerNum) {
    char playerNum_c[2] = {0};
    sprintf_s(playerNum_c, 2, "%01d", (int)playerNum);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 80, 39, playerNum_c, 1);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 80, 39, "  PLAYER GAME", 1);
}

static void drawVirusNumber(Graphics* graphics, uint32_t x, uint32_t y, size_t virusLevel) {
    char virusLevel_c[3] = {0};
    sprintf_s(virusLevel_c, 3, "%02d", (int)virusLevel);

    VT(Asset_DSNumberBox)->draw(&Asset_DSNumberBox, graphics, x, y, 20, 13);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, x + 5, y + 4, virusLevel_c, 1);
}


/* Drawing entry point */
static void draw(this_p(GameState)) {
    Graphics *graphics = VTP(this->engine->screen)->getGraphics(this->engine->screen);
    DifficultySelectionGameState *state = (DifficultySelectionGameState *) this;
    uint32_t x;

    /* Background */
    graphics->currentPalette = &Asset_DSMenuPalette;
    VTP(graphics)->drawCheckerboard(graphics, 8, 2, 0);
    VT(Asset_DSPanel)->draw(&Asset_DSPanel, graphics, 24, 24, 161);
    VTP(graphics)->drawTexture(graphics, &Asset_DSSlider, 89, 92, 89 + Asset_DSSlider.width, 92 + Asset_DSSlider.height, 0, 0, Asset_DSSlider.width, Asset_DSSlider.height);

    /* Virus Level */
    drawVirusNumber(graphics, 179, 75, state->playerInfos[0].virusLevel);
    if (state->multiplayer)
        drawVirusNumber(graphics, 179, 99, state->playerInfos[1].virusLevel);

    x = (uint32_t) state->playerInfos[0].virusLevel * 4;
    VTP(graphics)->drawTexture(graphics, &Asset_DSSelectorDownSM, 87 + x, 85,
                               87 + x + Asset_DSSelectorDownSM.width, 85 + Asset_DSSelectorDownSM.height,
                               0, 0, Asset_DSSelectorDownSM.width, Asset_DSSelectorDownSM.height);
    if (state->multiplayer) {
        x = (uint32_t) state->playerInfos[1].virusLevel * 4;
        VTP(graphics)->drawTexture(graphics, &Asset_DSSelectorUpSM, 87 + x, 99,
                                   87 + x + Asset_DSSelectorUpSM.width, 99 + Asset_DSSelectorUpSM.height,
                                   0, 0, Asset_DSSelectorUpSM.width, Asset_DSSelectorUpSM.height);
    }

    /* Speed */
    x = (uint32_t) (state->playerInfos[0].speed - 1) * 40;
    VTP(graphics)->drawTexture(graphics, &Asset_DSSelectorDownLG, 129 + x, 137,
                               129 + x + Asset_DSSelectorDownLG.width, 137 + Asset_DSSelectorDownLG.height,
                               0, 0, Asset_DSSelectorDownLG.width, Asset_DSSelectorDownLG.height);
    if (state->multiplayer) {
        x = (uint32_t) (state->playerInfos[1].speed - 1) * 40;
        VTP(graphics)->drawTexture(graphics, &Asset_DSSelectorUpLG, 129 + x, 152,
                                   129 + x + Asset_DSSelectorUpLG.width, 152 + Asset_DSSelectorUpLG.height,
                                   0, 0, Asset_DSSelectorUpLG.width, Asset_DSSelectorUpLG.height);
    }

    VT(Asset_DSUnselected)->draw(&Asset_DSUnselected, graphics, 44, 61, 90, 8);
    VT(Asset_DSUnselected)->draw(&Asset_DSUnselected, graphics, 44, 117, 42, 8);
    VT(Asset_DSUnselected)->draw(&Asset_DSUnselected, graphics, 44, 165, 82, 8);
    switch (state->selectedMenuEntry) {
        case DifficultySelectionGameState_VirusLevel:
            VT(Asset_DSSelected)->draw(&Asset_DSSelected, graphics, 40, 57, 90, 8);
            break;
        case DifficultySelectionGameState_Speed:
            VT(Asset_DSSelected)->draw(&Asset_DSSelected, graphics, 40, 113, 42, 8);
            break;
        case DifficultySelectionGameState_MusicType:
            VT(Asset_DSSelected)->draw(&Asset_DSSelected, graphics, 40, 161, 82, 8);
            break;
        default:
            break;
    }

    switch (state->music) {
        case Fever:
            VT(Asset_DSSubSel)->draw(&Asset_DSSubSel, graphics, 59, 187, 45, 12);
            break;
        case Chill:
            VT(Asset_DSSubSel)->draw(&Asset_DSSubSel, graphics, 115, 187, 45, 12);
            break;
        case Off:
            VT(Asset_DSSubSel)->draw(&Asset_DSSubSel, graphics, 170, 187, 29, 12);
            break;
        default:
            break;
    }

    /* Text */
    drawTitle(graphics, state->multiplayer ? 2 : 1);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 48, 63, "VIRUS LEVEL", 1);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 48, 119, "SPEED", 1);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 88, 143, "LOW", 1);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 128, 143, "MED", 1);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 168, 143, "HI", 1);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 48, 167, "MUSIC TYPE", 1);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 64, 191, "FEVER", 1);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 120, 191, "CHILL", 1);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 176, 191, "OFF", 1);

    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 64, 87, "1P", 1);
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 64, 139, "1P", 1);
    if (state->multiplayer) {
        VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 64, 95, "2P", 1);
        VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 64, 147, "2P", 1);
    }

#ifdef SAVE_DEBUG
    SDL_SaveBMP(((SDL_Graphics*)graphics)->screen->screenSurface, "a.bmp");
    ThrowError("debug");
#endif
}


/* *************************************************************** */
/* Game Logic                                                      */
/* *************************************************************** */

static void increment(this_p(DifficultySelectionGameState), int playerId) {
    switch (this->selectedMenuEntry) {
        case DifficultySelectionGameState_VirusLevel:
            if (this->playerInfos[playerId].virusLevel < SinglePlayerGame_MaxVirus) this->playerInfos[playerId].virusLevel++;
            break;
        case DifficultySelectionGameState_Speed:
            if (this->playerInfos[playerId].speed < 2) this->playerInfos[playerId].speed++;
            break;
        case DifficultySelectionGameState_MusicType:
            if (this->music < Off) this->music++;
            break;
        default:
            break;
    }
}

static void decrement(this_p(DifficultySelectionGameState), int playerId) {
    switch (this->selectedMenuEntry) {
        case DifficultySelectionGameState_VirusLevel:
            if (this->playerInfos[playerId].virusLevel > 0) this->playerInfos[playerId].virusLevel--;
            break;
        case DifficultySelectionGameState_Speed:
            if (this->playerInfos[playerId].speed > 0) this->playerInfos[playerId].speed--;
            break;
        case DifficultySelectionGameState_MusicType:
            if (this->music > Fever) this->music--;
            break;
        default:
            break;
    }
}

static void allKeysDown(this_p(GameState)) {
    VTP(this->engine->inputDevice)->reset(this->engine->inputDevice);
}

static void startSinglePlayer(this_p(DifficultySelectionGameState)) {
    SinglePlayerGameState singlePlayerGameState;
    SinglePlayerGame logic;
    GameBoard board;
    GameSpeedProvider speedProvider;
    size_t currentLevel;
    bool won = true;

    GameSpeedProvider_init(&speedProvider, (size_t) this->playerInfos[0].speed,
                           Default_FirstPillTimeout,
                           Default_NextPillDelay,
                           (uint32_t) (500 / ((this->playerInfos[0].speed + 1) * 1.5)),
                           Default_FallingGravityDelay);

    for (currentLevel = this->playerInfos[0].virusLevel; currentLevel <= 20 && won; currentLevel++) {
        GameBoard_init(&board);

        SinglePlayerGame_init(&logic, 0, currentLevel,
                              &speedProvider, &board);

        SinglePlayerGameState_init(&singlePlayerGameState, this->base.engine, &logic);
        VTP(this->base.engine)->loadState(this->base.engine, (GameState *) &singlePlayerGameState);
        won = (bool) (logic.state == SinglePlayerState_EndWon);
    }
}

static void startMultiPlayer(this_p(DifficultySelectionGameState)) {
	MultiPlayerGameState multiPlayerGameState;
	SinglePlayerGame logic1, logic2;
	GameBoard board1, board2;
	GameSpeedProvider speedProvider1, speedProvider2;
	size_t currentLevel1, currentLevel2;
	bool won = true;

	GameSpeedProvider_init(&speedProvider1, (size_t)this->playerInfos[0].speed,
		Default_FirstPillTimeout,
		Default_NextPillDelay,
		(uint32_t)(500 / ((this->playerInfos[0].speed + 1) * 1.5)),
		Default_FallingGravityDelay);
	GameSpeedProvider_init(&speedProvider2, (size_t)this->playerInfos[1].speed,
		Default_FirstPillTimeout,
		Default_NextPillDelay,
		(uint32_t)(500 / ((this->playerInfos[1].speed + 1) * 1.5)),
		Default_FallingGravityDelay);

	//for (currentLevel = this->playerInfos[0].virusLevel; currentLevel <= 20 && won; currentLevel++) {
		GameBoard_init(&board1);
		GameBoard_init(&board2);

		currentLevel1 = this->playerInfos[0].virusLevel;
		currentLevel2 = this->playerInfos[1].virusLevel;
		SinglePlayerGame_init(&logic1, 0, currentLevel1,
			&speedProvider1, &board1);
		SinglePlayerGame_init(&logic2, 0, currentLevel2,
			&speedProvider2, &board2);

		MultiPlayerGameState_init(&multiPlayerGameState, this->base.engine, &logic1, &logic2);
		VTP(this->base.engine)->loadState(this->base.engine, (GameState *)&multiPlayerGameState);
		//won = (bool)(logic1.state == SinglePlayerState_EndWon);
	//}
}



static bool update(this_p(GameState)) {
    DifficultySelectionGameState *state = (DifficultySelectionGameState *)this;
    InputState *inputState = VTP(this->engine->inputDevice)->getInputState(this->engine->inputDevice);

    if (inputState->direction1 == PillDirection_Down) {
        state->selectedMenuEntry += state->selectedMenuEntry < DifficultySelectionGameState_MusicType ? 1 : 0;
        allKeysDown(this);
    } else if (inputState->direction1 == PillDirection_Up) {
        state->selectedMenuEntry -= state->selectedMenuEntry > DifficultySelectionGameState_VirusLevel ? 1 : 0;
        allKeysDown(this);
    }

    if (inputState->direction1 == PillDirection_Right) {
        increment(state, 0);
        allKeysDown(this);
    } else if (inputState->direction1 == Left) {
        decrement(state, 0);
        allKeysDown(this);
    }
    if (inputState->direction2 == PillDirection_Right) {
        increment(state, 1);
        allKeysDown(this);
    } else if (inputState->direction2 == PillDirection_Left) {
        decrement(state, 1);
        allKeysDown(this);
    }

    if (inputState->enterButton) {
        if (state->multiplayer) {
			startMultiPlayer(state);
        } else {
            startSinglePlayer(state);
        }
    }

    return true;
}


/* *************************************************************** */
/* Initialization                                                  */
/* *************************************************************** */

static struct GameState_VTABLE _vtable = {
        update, draw
};

void DifficultySelectionGameState_init(this_p(DifficultySelectionGameState), Engine *engine, bool multiplayer) {
    this->base.engine = engine;
    VT(this->base) = &_vtable;
    this->multiplayer = multiplayer;
    this->selectedMenuEntry = DifficultySelectionGameState_VirusLevel;
    this->music = Fever;
    memset(this->playerInfos, 0, sizeof(DifficultySelectionGameState_PlayerInfo) * 2);
}
