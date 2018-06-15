/*
 *  FileGameBoard.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef FILEGAMEBOARD_H
#define FILEGAMEBOARD_H

#include "Object.h"
#include "Bool.h"
#include "Vector.h"
#include "IntTypes.h"
#include "FileGameBoard.h"

typedef class FileGameBoard {
    GameBoard base;

    char *filePath;
} FileGameBoard;

void FileGameBoard_init(this_p(FileGameBoard), char *filePath);

#endif
