/*
 *  Font.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef FONT_H
#define FONT_H

#include "Texture.h"
#include "Object.h"

typedef class Font {
    Texture* texture;
    unsigned int charWidth;
    int* charMap;
} Font;

#endif
