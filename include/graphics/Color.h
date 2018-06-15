/*
 *  Color.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  Colors are defined as ARGB (8 bit per channel) packed into an int (BGRA for endiannes)
 */

#ifndef COLOR_H
#define COLOR_H

#include "CIntTypes.h"

typedef unsigned char ColorIndex;

typedef struct Color {
    unsigned char b, g, r, a;
} Color;

Color Color_FromInt(uint32_t code);

#endif
