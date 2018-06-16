/*
 *  SinglePlayerGame.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "SinglePlayerGame.h"
#include <time.h>

static uint32_t randomBetween(size_t min, size_t max) {
    return (uint32_t)((rand() % (max - min)) + min);
}


/* For board init animation */
static bool addNextVirus(this_p(SinglePlayerGame)) {
    if (this->board->virusCount < this->virusCount) {
        VTP(this->board)->addRandomVirus(this->board);
        return true;
    } else return false;
}

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

static void updateScore(this_p(SinglePlayerGame)) {
	for (int i = 1; i <= this->deletedVirusCount; i++)
		this->score += 100 * (1 << i);

	this->deletedVirusCount = 0;
}

static void gravityTimeoutCallback(this_p(SinglePlayerGame)) {
    if (this->state == SinglePlayerState_Moving || this->state == SinglePlayerState_NoControl) {
        if (this->nextAction == SinglePlayerAction_Remove) {
            int removedVirus = 0;
            bool removed = VTP(this->board)->removeFirst(this->board, &removedVirus);
            if (removed) {
                this->state = SinglePlayerState_NoControl;
                this->virusCount -= removedVirus;
                this->deletedVirusCount += removedVirus;
            } else {
                if (!this->lastActionResult) {
                    this->state = SinglePlayerState_WaitingForPill;
                    this->nextAction = SinglePlayerAction_Gravity;
                }
            }

            this->lastAction = SinglePlayerAction_Remove;
            this->nextAction = SinglePlayerAction_Gravity;
        } else if (this->nextAction == SinglePlayerAction_Gravity) {
            bool gResult = VTP(this->board)->applyGravity(this->board, this->currentPillId);
            this->lastAction = SinglePlayerAction_Gravity;
            this->nextAction = SinglePlayerAction_Remove;

            this->lastActionResult = gResult;
        }
    }
}

static int getGravityTimeoutTime(this_p(SinglePlayerGame)) {
    if (this->state == SinglePlayerState_NoControl) return 70; /* faster for non user pill */
    else return 500 / ((this->speed + 1) * 1.5);
}


static void update(this_p(SinglePlayerGame), Engine* engine, PillDirection direction) {
    uint32_t time = VTP(engine->screen)->getCurrentTime(engine->screen);

    /* win condition */
    if (this->virusCount <= 0) {
        this->state = SinglePlayerState_EndWon;
    }

    /* gravity timeout */
    if (time - this->lastGravityTime > getGravityTimeoutTime(this)) {
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
			updateScore(this);

			this->nextPillColorL = (GameBoardElementColor)(randomBetween(0, 3));
			this->nextPillColorR = (GameBoardElementColor)(randomBetween(0, 3));
			this->state = SinglePlayerState_Moving;
            this->lastGravityTime = VTP(engine->screen)->getCurrentTime(engine->screen);
		} else {
			this->state = SinglePlayerState_EndLost;
		}
	}
}


/* *************************************************************** */
/* Constructor                                                     */
/* *************************************************************** */

static struct SinglePlayerGame_VTABLE _vtable = {
        addNextVirus, update
};

void SinglePlayerGame_init(this_p(SinglePlayerGame), Engine* engine, size_t top, size_t level, size_t virus,
                           SinglePlayerGame_Speed speed, GameBoard *board) {
	VTP(this) = &_vtable;
    this->state = SinglePlayerState_FillingBoard;
    this->top = top;
    this->score = 0;
    this->level = level;
    this->virusCount = 4 * (virus + 1);
    this->speed = speed;
	this->deletedVirusCount = 0;
    this->board = board;
    this->lastActionResult = true;
    this->lastAction = SinglePlayerAction_Gravity;
    this->nextAction = SinglePlayerAction_Gravity;

    srand((unsigned int) time(NULL));

    /* Next pill */
	this->nextPillColorL = (GameBoardElementColor)(randomBetween(0, 3));
	this->nextPillColorR = (GameBoardElementColor)(randomBetween(0, 3));
    this->currentPillId = -1;
}
