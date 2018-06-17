/*
 *  FileGameBoard.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  An extension of GameBoard which reads from file
 */

#ifndef FILEGAMEBOARD_H
#define FILEGAMEBOARD_H

#include "Object.h"
#include "Bool.h"
#include "Vector.h"
#include "CIntTypes.h"
#include "GameBoard.h"

/* Expected file size to read */
#define FileBufferSize ((BoardWidth + 1) * BoardHeight - 1)

typedef class FileGameBoard {
    GameBoard base;

    char *filePath;
    char fileContent[FileBufferSize]; /* readt file content */
    size_t lastVirusPos, lastX, lastY, fileVirusCount;
} FileGameBoard;

void FileGameBoard_init(this_p(FileGameBoard), Random *random, char *filePath);

#endif
