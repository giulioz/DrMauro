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
    SinglePlayerGame *logic1, *logic2;
    SinglePlayerGame_State lastLogicState1, lastLogicState2;

} MultiPlayerGameState;

void MultiPlayerGameState_init(this_p(MultiPlayerGameState), Engine *engine, SinglePlayerGame *logic1, SinglePlayerGame *logic2);

#endif
