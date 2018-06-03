/*
 *  Palettes.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "Assets.h"

static unsigned char med_colors[][4] = {
        {0, 0, 0, 0},
        {255, 255, 255, 255},
        {164, 0, 59, 0},        /* checkerboard */

        {123, 30, 183, 255},    /* purple */
        {255, 144, 146, 255},   /* light purple */
        {255, 210, 211, 255},   /* light light purple */
        {255, 176, 100, 255},   /* light blue */
        {255, 223, 192, 255},   /* light light blue */
        {222, 205, 72, 255},    /* light teal */
        {0, 190, 188, 255},     /* yellow */
        {0, 109, 107, 255},     /* dark yellow */
        {165, 216, 247, 255},   /* light yellow */
        {102, 102, 102, 255},   /* grey */
        {0, 0, 0, 255},         /* black */
        {164, 0, 59, 255},      /* purple-blue */
};

Palette Asset_MedPalette = {
        (int *) &med_colors
};

static unsigned char menu_colors[][4] = {
        {0, 82, 0, 0},
        {255, 255, 255, 255},
        {0, 148, 16, 0},        /* checkerboard */

        {123, 30, 183, 255},    /* purple */
        {255, 144, 146, 255},   /* light purple */
        {255, 210, 211, 255},   /* light light purple */
        {255, 176, 100, 255},   /* light blue */
        {255, 223, 192, 255},   /* light light blue */
        {222, 205, 72, 255},    /* light teal */
        {0, 190, 188, 255},     /* yellow */
        {0, 109, 107, 255},     /* dark yellow */
        {165, 216, 247, 255},   /* light yellow */
        {102, 102, 102, 255},   /* grey */
        {0, 0, 0, 255},         /* black */
        {164, 0, 59, 255},      /* purple-blue */
};

Palette Asset_MenuPalette = {
        (int *) &menu_colors
};
