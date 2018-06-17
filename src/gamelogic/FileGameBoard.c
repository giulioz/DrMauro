/*
 *  FileGameBoard.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "FileGameBoard.h"

#include "ExceptionManager.h"
#include <stdio.h>

/* fills the game board with the viruses loaded from a file */
static GameBoardElement* addRandomVirus(this_p(GameBoard)) {
    FileGameBoard *fileGameBoard = (FileGameBoard *) this;
    size_t j;
    size_t x = fileGameBoard->lastX, y = fileGameBoard->lastY;

    for (j = fileGameBoard->lastVirusPos; j < FileBufferSize; j++) {
        char val = fileGameBoard->fileContent[j];

        if (val == '\n') {
            y--;
            x = 0;
        } else if (val == 'R' || val == 'G' || val == 'B') {
            GameBoardElement *element = VT(this->board)->get2D(&this->board, y, x);

            if (val == 'R') {
                element->id = -1;
                element->type = GameBoardElement_Virus;
                element->color = GameBoardElement_Red;
            } else if (val == 'G') {
                element->id = -1;
                element->type = GameBoardElement_Virus;
                element->color = GameBoardElement_Yellow;
            } else if (val == 'B') {
                element->id = -1;
                element->type = GameBoardElement_Virus;
                element->color = GameBoardElement_Blue;
            }

            fileGameBoard->lastVirusPos = j + 1;
            fileGameBoard->lastX = x + 1;
            fileGameBoard->lastY = y;
            this->virusCount++;
            this->lastAddedVirus = element;
            this->lastAddedX = (uint32_t) x;
            this->lastAddedY = (uint32_t) y;
            return element;
        } else {
            x++;
        }
    }

    return NULL;
}

/* open a file containing a game board, raises
   an exception if it encounter errors */
static void readFile(this_p(FileGameBoard)) {
    FILE *file;
    size_t readt = 0;

    if (!(file = fopen(this->filePath, "r")))
        ThrowError("Cannot read board file");

    readt = fread(this->fileContent, sizeof(char), FileBufferSize, file);
    if (readt != FileBufferSize)
        ThrowError("Board file size incorrect");

    fclose(file);
}

static size_t getVirusCount(this_p(FileGameBoard)) {
    size_t i, count = 0;

    for (i = 0; i < FileBufferSize; i++) {
        char val = this->fileContent[i];
        if (val == 'R' || val == 'G' || val == 'B')
            count++;
    }

    return count;
}


static struct GameBoard_VTABLE _vtable;

void FileGameBoard_init(this_p(FileGameBoard), Random *random, char *filePath) {
    GameBoard_init(&this->base, random);

    _vtable = *this->base.VTABLE;
    VT(this->base) = &_vtable;
    VT(this->base)->addRandomVirus = addRandomVirus; /* override */

    this->filePath = filePath;
    this->lastVirusPos = 0;
    this->lastY = BoardHeight - 1;
    this->lastX = 0;
    readFile(this);
    this->fileVirusCount = getVirusCount(this);
}
