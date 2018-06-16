/*
 *  DifficultySelectionGameState.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef DIFFICULTYSELECTIONSAMESTATE_H
#define DIFFICULTYSELECTIONSAMESTATE_H

#include "Object.h"
#include "GameState.h"
#include "Graphics.h"
#include "Engine.h"
#include "Assets.h"
#include "Bool.h"
#include "Sprite.h"
#include "Vector.h"
#include "CIntTypes.h"
#include "Timeline.h"
#include "SinglePlayerGameState.h"
#include "MultiPlayerGameState.h"
#include "GameBoard.h"
#include "SinglePlayerGame.h"

typedef enum {
    DifficultySelectionGameState_VirusLevel,
    DifficultySelectionGameState_Speed,
    DifficultySelectionGameState_MusicType
} DifficultySelectionGameState_SelectedEntry;

typedef struct {
    size_t virusLevel;
    int speed;
} DifficultySelectionGameState_PlayerInfo;

typedef enum {
    Fever,
    Chill,
    Off
} DifficultySelectionGameState_SelectedMusic;

typedef class DifficultySelectionGameState {
    GameState base;

    DifficultySelectionGameState_SelectedEntry selectedMenuEntry;
    bool multiplayer;
    DifficultySelectionGameState_PlayerInfo playerInfos[2];
    DifficultySelectionGameState_SelectedMusic music;

} DifficultySelectionGameState;

void DifficultySelectionGameState_init(this_p(DifficultySelectionGameState), Engine *engine, bool multiplayer);

#endif
