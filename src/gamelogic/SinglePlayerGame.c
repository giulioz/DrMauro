/*
 *  SinglePlayerGame.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "SinglePlayerGame.h"
#include <string.h>
#include <time.h>
#include <stdio.h>

static uint32_t randomBetween(size_t min, size_t max) {
    return (uint32_t)((rand() % (max - min)) + min);
}


/* For board init animation */
static bool addNextVirus(this_p(SinglePlayerGame), uint32_t i) {
    if (i < this->virusCount) {
        while (!VT(this->board)->addRandomVirus(&this->board, i));
		return true;
	} else {
		return false;
	}
}

static bool addNextPill(this_p(SinglePlayerGame)) {
    GameBoardElement *r, *l;
    l = VT(this->board)->getElement(&this->board, 3, this->board.board.height - 1);
    r = VT(this->board)->getElement(&this->board, 4, this->board.board.height - 1);

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


static void update(this_p(SinglePlayerGame), Engine* engine, PillDirection direction) {
    uint32_t time = VTP(engine->screen)->getCurrentTime(engine->screen);

    /* win condition */
    if (this->virusCount <= 0) {
        this->state = SinglePlayerState_EndWon;
    }

    if (this->state == SinglePlayerState_Moving) {
        VT(this->board)->pillMove(&this->board, this->currentPillId, direction);
    }

    if (this->state == SinglePlayerState_Moving || this->state == SinglePlayerState_Still) {
        if (time - this->lastGravity >
            (this->state == SinglePlayerState_Still ? 300 : 2000) / ((this->speed + 1) * 1.5)) {
            if (!VT(this->board)->applyGravity(&this->board, this->currentPillId)) {
                this->state = SinglePlayerState_Still;
                int removedVirus = VT(this->board)->removeFirst(&this->board);
                if (!removedVirus)
                    this->state = SinglePlayerState_Ready;
                this->virusCount -= removedVirus;
                this->deletedVirusCount += removedVirus;
            }
            this->lastGravity = time;
        }
    }

    /* create new pill */
	if (this->state == SinglePlayerState_Ready) {
		if (addNextPill(this)) {
			updateScore(this);

			this->nextPillColorL = (GameBoardElementColor)(randomBetween(0, 3));
			this->nextPillColorR = (GameBoardElementColor)(randomBetween(0, 3));
			this->state = SinglePlayerState_Moving;
		} else {
			this->state = SinglePlayerState_EndLost;
		}
	}
}


static void startGame(this_p(SinglePlayerGame), Engine* engine) {
    addNextPill(this);
    this->state = SinglePlayerState_Moving;
    this->lastGravity = VTP(engine->screen)->getCurrentTime(engine->screen);
}


/* *************************************************************** */
/* Constructor                                                     */
/* *************************************************************** */

static struct SinglePlayerGame_VTABLE _vtable = {
        addNextVirus, startGame, update
};

void SinglePlayerGame_init(this_p(SinglePlayerGame), Engine* engine, int top, int level, int virus, SinglePlayerGame_Speed speed) {
	VTP(this) = &_vtable;
    this->state = SinglePlayerState_FillingBoard;
    this->top = top;
    this->score = 0;
    this->level = level;
    this->virusCount = 4 * (virus + 1);
    this->speed = speed;
	this->deletedVirusCount = 0;

    srand((unsigned int) time(NULL));

    /* Next pill */
	this->nextPillColorL = (GameBoardElementColor)(randomBetween(0, 3));
	this->nextPillColorR = (GameBoardElementColor)(randomBetween(0, 3));
    this->currentPillId = -1;

    /* Board */
    GameBoard_init(&this->board);
}
