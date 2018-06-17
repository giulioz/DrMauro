/*
 *  BootState.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include <time.h>
#include "BootState.h"

static void startMenu(this_p(GameState)) {
    MainMenuGameState nextState;

    MainMenuGameState_init(&nextState, this->engine);
    VTP(this->engine)->loadState(this->engine, (GameState*)&nextState);
}

static void startGameWithDifficulty(this_p(GameState)) {
    SinglePlayerGameState singlePlayerGameState;
    SinglePlayerGame logic;
    GameBoard board;
    GameSpeedProvider speedProvider;
    Random random;

    Random_init(&random, (uint32_t) time(NULL));
    GameBoard_init(&board, &random);

    GameSpeedProvider_init(&speedProvider, 1, /* UI will be set to MED */
                           Default_FirstPillTimeout,
                           Default_NextPillDelay,
                           this->engine->parameters->speed,
                           Default_FallingGravityDelay);

    SinglePlayerGame_init(&logic, 0, this->engine->parameters->difficulty,
                          &speedProvider, &board);

    SinglePlayerGameState_init(&singlePlayerGameState, this->engine, &logic);
    VTP(this->engine)->startup(this->engine, (GameState *) &singlePlayerGameState);
}

static void startGameWithCustomBoard(this_p(GameState)) {
    SinglePlayerGameState singlePlayerGameState;
    SinglePlayerGame logic;
    FileGameBoard board;
    GameSpeedProvider speedProvider;
    Random random;

    Random_init(&random, (uint32_t) time(NULL));
    FileGameBoard_init(&board, &random, this->engine->parameters->boardFile);

    GameSpeedProvider_init(&speedProvider, 1, /* UI will be set to MED */
                           Default_FirstPillTimeout,
                           Default_NextPillDelay,
                           this->engine->parameters->speed,
                           Default_FallingGravityDelay);

    SinglePlayerGame_init(&logic, 0, 0, &speedProvider, (GameBoard *) &board);
    logic.virusCount = board.fileVirusCount;

    SinglePlayerGameState_init(&singlePlayerGameState, this->engine, &logic);
    VTP(this->engine)->startup(this->engine, (GameState *) &singlePlayerGameState);
}


static bool update(this_p(GameState)) {
    /* stay always active */
    return true;
}

static void draw(this_p(GameState)) {
    Graphics *graphics = VTP(this->engine->screen)->getGraphics(this->engine->screen);

    /* Loading screen (nearly useless) */
    VTP(graphics)->drawString(graphics, &Asset_DefaultFont, 10, 10, "Loading", 1);

    switch (this->engine->parameters->type) {
        case GameType_CustomParams:
            startGameWithDifficulty(this);
            break;
        case GameType_CustomBoard:
            startGameWithCustomBoard(this);
            break;
        default:
            startMenu(this);
            break;
    }
}

static struct GameState_VTABLE _vtable = {
        update, draw
};

void BootState_init(this_p(BootState), Engine *engine) {
    this->base.engine = engine;
    VT(this->base) = &_vtable;
}
