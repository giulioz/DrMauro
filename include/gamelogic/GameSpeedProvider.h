/*
 *  GameSpeedProvider.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  Contains time delays for game events
 */

#ifndef GAMESPEEDPROVIDER_H
#define GAMESPEEDPROVIDER_H

#include "CIntTypes.h"
#include "Object.h"
#include "SinglePlayerGame_State.h"

#define Default_NextPillDelay 300
#define Default_FallingGravityDelay 50

class GameSpeedProvider;

struct GameSpeedProvider_VTABLE {
    uint32_t (*getGravityTime)(this_p(GameSpeedProvider), SinglePlayerGame_State state);
};

typedef class GameSpeedProvider {
    struct GameSpeedProvider_VTABLE *VTABLE;

    uint32_t nextPillDelay, gravityDelay, fallingGravityDelay;
    size_t speed; /* 0: LOW, 1: MED, 2: HI */
} GameSpeedProvider;

void GameSpeedProvider_init(this_p(GameSpeedProvider), size_t speed,
                            uint32_t nextPillDelay, uint32_t gravityDelay, uint32_t fallingGravityDelay);

#endif
