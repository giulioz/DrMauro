/*
 *  GameSpeedProvider.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  Contains time delays for game events
 */

#include "GameSpeedProvider.h"

uint32_t getGravityTime(this_p(GameSpeedProvider), SinglePlayerGame_State state) {
    if (state == SinglePlayerState_NoControl) return this->fallingGravityDelay; /* faster for non user pill */
    else return this->gravityDelay;
}


static struct GameSpeedProvider_VTABLE _vtable = {
        getGravityTime
};

void GameSpeedProvider_init(this_p(GameSpeedProvider), size_t speed, uint32_t firstPillTimeout,
                            uint32_t nextPillDelay, uint32_t gravityDelay, uint32_t fallingGravityDelay) {
    VTP(this) = &_vtable;
    this->speed = speed;
    this->firstPillTimeout = firstPillTimeout;
    this->nextPillDelay = nextPillDelay;
    this->gravityDelay = gravityDelay;
    this->fallingGravityDelay = fallingGravityDelay;
}
