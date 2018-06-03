/*
 *  MainMenuGameState.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef MAINMENUGAMESTATE_H
#define MAINMENUGAMESTATE_H

#include "Object.h"
#include "GameState.h"
#include "Graphics.h"
#include "Engine.h"
#include "Assets.h"
#include "Bool.h"
#include "Sprite.h"
#include "Vector.h"
#include "IntTypes.h"
#include "Timeline.h"
#include "SinglePlayerGameState.h"

typedef class MainMenuGameState {
    GameState base;

    SinglePlayerGameState singlePlayerGameState;

    /* Sprites */
    Sprite marioSprite, virusSprite, titleSprite, selectorSprite;

} MainMenuGameState;

void MainMenuGameState_init(this_p(MainMenuGameState), Engine *engine);

#endif
