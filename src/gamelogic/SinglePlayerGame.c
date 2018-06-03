/*
 *  SinglePlayerGame.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "SinglePlayerGame.h"
#include <string.h>

static void update(this_p(SinglePlayerGame), Engine* engine) {
    uint32_t time = VTP(engine->screen)->getCurrentTime(engine->screen);
	this->score++;
}

static void initBoard(this_p(SinglePlayerGame), int virus) {
    uint32_t x, y;

    for (x = 0; x < this->board.width; x++) {
        for (y = 0; y < this->board.height - 4; y++) {
            GameBoardElement *element = VT(this->board)->get2D(&this->board, y, x);
            element->type = rand() % 3;
            element->color = rand() % 3;
            element->id = rand() % 5;
        }
    }
}


static struct SinglePlayerGame_VTABLE _vtable = {
        update
};

void SinglePlayerGame_init(this_p(SinglePlayerGame), int top, int level, int virus, SinglePlayerGame_Speed speed) {
    VTP(this) = &_vtable;
    this->state = SinglePlayerState_Playing;
    this->top = top;
    this->score = 0;
    this->level = level;
    this->virus = virus;
    this->speed = speed;

    /* Board */
    StackVector2D_init(&this->board, SPBoardWidth, SPBoardHeight, sizeof(GameBoardElement), this->boardAlloc);
    memset(this->boardAlloc, 0, sizeof(GameBoardElement) * SPBoardWidth * SPBoardHeight);
    initBoard(this, virus);
}
