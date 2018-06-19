/*
 *  SinglePlayerGame.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  Manages a player and the gameplay
 *
 *  Va bene sia per il single player che per il multiplayer.
 *  Lo so, avrei dovuto fare un'altra classe che eredita e fare l'override di alcune cose invece che fare sta schifezza,
 *  ma sono le 16:34 e la consegna è alle 00, quindi devo muovermi.
 *  Saluti e tanti auguri a leggere più di 5000 righe di codice scritte con i piedi.
 */

#include "SinglePlayerGame.h"


/* for board init animation, returns false if virus number reached */
static bool addNextVirus(this_p(SinglePlayerGame)) {
    if (this->board->virusCount < this->virusCount) {
        VTP(this->board)->addRandomVirus(this->board);
        return true;
    } else return false;
}

/* new pill on top, false if loose */
static bool addNextPill(this_p(SinglePlayerGame)) {
    GameBoardElement *r, *l;
    l = VTP(this->board)->getElement(this->board, 3, this->board->board.height - 1);
    r = VTP(this->board)->getElement(this->board, 4, this->board->board.height - 1);

    if (r->type != GameBoardElement_Empty || l->type != GameBoardElement_Empty)
        return false; /* occupied, lose */
    else {
        this->currentPillId++;
        l->type = GameBoardElement_Pill;
        r->type = GameBoardElement_Pill;
        l->color = this->nextPillColorL;
        r->color = this->nextPillColorR;
        l->id = this->currentPillId;
        r->id = this->currentPillId;
        return true;
    }
}

/* update the score and increase the multiplier */
static void updateScore(this_p(SinglePlayerGame)) {
    for (int i = 1; i <= this->deletedVirusCount; i++)
        this->score += (100 * (1 << i)) * this->scoreMultiplier;

    this->scoreMultiplier *= 2;
    this->deletedVirusCount = 0;
}


/* update gravity every time */
static void gravityTimeoutCallback(this_p(SinglePlayerGame)) {
    if (this->state == SinglePlayerState_Moving || this->state == SinglePlayerState_NoControl) {
        if (this->nextAction == SinglePlayerAction_Remove) {
            int removedVirus = 0;
            bool removed = VTP(this->board)->removeFirst(this->board, &removedVirus, &this->lastVirusRemovedColor);
            if (removed) {
                this->state = SinglePlayerState_NoControl;
                this->virusCount -= removedVirus;
                this->deletedVirusCount += removedVirus;

                if (this->deletedVirusCount != 0)
                    updateScore(this);
            } else {
                if (!this->lastActionResult) {
                    this->state = SinglePlayerState_WaitingForPill;
                    this->nextAction = SinglePlayerAction_Gravity;
                }
            }

            this->nextAction = SinglePlayerAction_Gravity;
        } else if (this->nextAction == SinglePlayerAction_Gravity) {
            bool gResult = VTP(this->board)->applyGravity(this->board, this->currentPillId);
            this->nextAction = SinglePlayerAction_Remove;
            
            if (!gResult)
                this->state = SinglePlayerState_NoControl;

            this->lastActionResult = gResult;
        }
    }
}

/* callback from frontend */
static void update(this_p(SinglePlayerGame), Engine* engine, PillDirection direction) {
    uint32_t time = VTP(engine->screen)->getCurrentTime(engine->screen);

    /* filling board */
    if (this->state == SinglePlayerState_FillingBoard) {
        if (!addNextVirus(this)) {
            this->state = SinglePlayerState_WaitingForPill; /* ready for begin the game */
        }
    }

    /* win condition */
    if (this->virusCount <= 0) {
        this->state = SinglePlayerState_EndWon;
    }

    /* gravity timeout */
    if (time - this->lastGravityTime > VTP(this->speedProvider)->getGravityTime(this->speedProvider, this->state)) {
        gravityTimeoutCallback(this);
        this->lastGravityTime = time;
    }

    /* keyboard movement */
    if (this->state == SinglePlayerState_Moving) {
        VTP(this->board)->pillMove(this->board, this->currentPillId, direction);
    }

    /* create new pill */
    if (this->state == SinglePlayerState_Ready) {
        if (addNextPill(this)) {
            this->nextPillColorL =
                    (GameBoardElementColor)(VTP(this->board->random)->randomBetween(this->board->random, 0, 3));
            this->nextPillColorR =
                    (GameBoardElementColor)(VTP(this->board->random)->randomBetween(this->board->random, 0, 3));
            this->state = SinglePlayerState_Moving;
            this->lastGravityTime = VTP(engine->screen)->getCurrentTime(engine->screen);
            this->scoreMultiplier = 1;
        } else {
            this->state = SinglePlayerState_EndLost;
        }
    }
}


/* *************************************************************** */
/* Constructor                                                     */
/* *************************************************************** */

static struct SinglePlayerGame_VTABLE _vtable = {
        update
};

void SinglePlayerGame_init(this_p(SinglePlayerGame), size_t top, size_t level,
                           GameSpeedProvider *speedProvider, GameBoard *board) {
    VTP(this) = &_vtable;
    this->state = SinglePlayerState_FillingBoard;
    this->top = top;
    this->score = 0;
    this->level = level;
    this->virusCount = 4 * (level + 1);
    this->board = board;
    this->speedProvider = speedProvider;

    this->deletedVirusCount = 0;
    this->scoreMultiplier = 1;

    this->lastActionResult = true;
    this->nextAction = SinglePlayerAction_Gravity;

    /* Next pill */
    this->nextPillColorL = (GameBoardElementColor)(VTP(this->board->random)->randomBetween(this->board->random, 0, 3));
    this->nextPillColorR = (GameBoardElementColor)(VTP(this->board->random)->randomBetween(this->board->random, 0, 3));
    this->currentPillId = -1;

    this->lastVirusRemovedColor = GameBoardElement_NoColor;
}
