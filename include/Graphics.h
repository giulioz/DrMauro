/*
 *  Graphics.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  Graphics functions
 */

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdlib.h>

#include "Bool.h"
#include "Object.h"
#include "Texture.h"
#include "Sprite.h"
#include "Color.h"

class Graphics;

struct Graphics_VTABLE {
    void (*drawCheckerboard)(this_p(Graphics), int step, Color colorA, Color colorB);
};

typedef class Graphics {
    struct Graphics_VTABLE *VTABLE;
} Graphics;

#endif
