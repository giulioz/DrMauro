/*
 *  SinglePlayerGame_State.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef SINGLEPLAYERGAME_STATE_H
#define SINGLEPLAYERGAME_STATE_H

typedef enum {
    SinglePlayerState_FillingBoard,     /* adding viruses for animation */
    SinglePlayerState_WaitingForPill,	/* game start timeout */
    SinglePlayerState_Moving,           /* current pill going down, user control */
    SinglePlayerState_NoControl,        /* falling and elimination, no user control */
    SinglePlayerState_Ready,            /* pill arrived, add it to the top */
    SinglePlayerState_EndWon,
    SinglePlayerState_EndLost,
    SinglePlayerState_Nothing
} SinglePlayerGame_State;

#endif
