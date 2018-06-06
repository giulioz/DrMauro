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
#include "DifficultySelectionGameState.h"

typedef class MainMenuGameState {
    GameState base;

    /* Next state */
    DifficultySelectionGameState difficultySelectionGameState;

    Sprite marioSprite, virusSprite, titleSprite, selectorSprite;
    int selectedMenuEntry;

} MainMenuGameState;

void MainMenuGameState_init(this_p(MainMenuGameState), Engine *engine);

#endif
