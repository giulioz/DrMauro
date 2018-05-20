/*
 *  Color.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  Colors are defined as RGBA (8 bit per channel) packed into an int
 */

#ifndef COLOR_H
#define COLOR_H

#include "IntTypes.h"

typedef struct Color {
    unsigned char r, g, b, a;
} Color;

Color Color_FromInt(uint32_t code);

#endif
