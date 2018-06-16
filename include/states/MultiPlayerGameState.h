/*
 *  MultiPlayerGameState.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  View part of a Multi Player Game
 */

#ifndef MULTIPLAYERGAMESTATE_H
#define MULTIPLAYERGAMESTATE_H

#include "Object.h"
#include "GameState.h"
#include "Graphics.h"
#include "Engine.h"
#include "Assets.h"
#include "Bool.h"
#include "Sprite.h"
#include "SinglePlayerGame.h"
#include "Vector.h"
#include "CIntTypes.h"
#include "Timeline.h"
#include "GameBoard.h"
#include "CommonGameUtils.h"

typedef class MultiPlayerGameState {
    GameState base;

    /* Timeline */
    Timeline timeline;

    /* Game logic */
    SinglePlayerGame logic;
    SinglePlayerGame_State lastLogicState;

    /* Sprites */
    Sprite marioSprite;
    Sprite virusLargeBlueSprite, virusLargeYellowSprite, virusLargeRedSprite;

} MultiPlayerGameState;

void MultiPlayerGameState_init(this_p(MultiPlayerGameState), Engine *engine, int top, int level, int virus, int speed);

#endif
