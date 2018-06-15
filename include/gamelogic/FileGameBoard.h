/*
 *  FileGameBoard.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef FILEGAMEBOARD_H
#define FILEGAMEBOARD_H

#include "Object.h"
#include "Bool.h"
#include "Vector.h"
#include "CIntTypes.h"
#include "GameBoard.h"

#define FileBufferSize ((BoardWidth + 1) * BoardHeight - 1)

typedef class FileGameBoard {
    GameBoard base;

    char *filePath;
    char fileContent[FileBufferSize];
    size_t lastVirusPos, lastX, lastY, fileVirusCount;
} FileGameBoard;

void FileGameBoard_init(this_p(FileGameBoard), char *filePath);

#endif
