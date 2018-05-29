/*
 *  SinglePlayerGame.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef SINGLEPLAYERGAME_H
#define SINGLEPLAYERGAME_H

#include "Object.h"
#include "Bool.h"

typedef enum {
    PLAYING,
    END_WON,
    END_LOOSE,
} SinglePlayerGame_State;

typedef enum {
    LOW,
    MED,
    HI
} SinglePlayerGame_Speed;

class SinglePlayerGame;

struct SinglePlayerGame_VTABLE {

};

typedef class SinglePlayerGame {
    struct SinglePlayerGame_VTABLE *VTABLE;

    SinglePlayerGame_State state;
    int top, score;
    int level, virus;
    SinglePlayerGame_Speed speed;
} SinglePlayerGame;

void SinglePlayerGame_init(this_p(SinglePlayerGame));

#endif
