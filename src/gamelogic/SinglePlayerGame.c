/*
 *  SinglePlayerGame.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "SinglePlayerGame.h"

static void newGame(this_p(SinglePlayerGame)) {

}

static void update(this_p(SinglePlayerGame), uint32_t time) {

}


static struct SinglePlayerGame_VTABLE _vtable = {
        newGame, update
};

void SinglePlayerGame_init(this_p(SinglePlayerGame)) {
    VTP(this) = &_vtable;
    this->state = END_LOOSE;
    this->top = 0;
    this->score = 0;
    this->level = 0;
    this->virus = 0;
    this->speed = MED;
}
