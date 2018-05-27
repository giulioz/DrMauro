/*
 *  Palettes.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "Assets.h"

static unsigned char med_colors[][4] = {
        {0, 0, 0, 0},
        {255, 255, 255, 255},
        {164, 0, 59, 0},

        {123, 30, 183, 255},
        {255, 144, 146, 255},
        {255, 210, 211, 255},
        {255, 176, 100, 255},
        {255, 223, 192, 255},
        {222, 205, 72, 255},
        {0, 190, 188, 255},
        {0, 109, 107, 255},
        {165, 216, 247, 255},
        {102, 102, 102, 255},
        {0, 0, 0, 255},
        {164, 0, 59, 255},
};

Palette Asset_DefaultPalette = {
        (int *) &med_colors
};
