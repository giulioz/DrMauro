/*
 *  SinglePlayerGame.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "SinglePlayerGame.h"

static void update(this_p(SinglePlayerGame), uint32_t time) {
	this->score++;
}


static struct SinglePlayerGame_VTABLE _vtable = {
        update
};

void SinglePlayerGame_init(this_p(SinglePlayerGame), int top, int level, int virus, SinglePlayerGame_Speed speed) {
    VTP(this) = &_vtable;
    this->state = PLAYING;
    this->top = top;
    this->score = 0;
    this->level = level;
    this->virus = virus;
    this->speed = speed;
}
