/*
 *  Font.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef FONT_H
#define FONT_H

#include "Texture.h"

typedef struct {
    Texture* Texture;
    unsigned int CharWidth;
} Font;

#endif
