/*
 *  Graphics.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  Graphics functions
 */

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Bool.h"
#include "Object.h"
#include "Texture.h"
#include "Sprite.h"
#include "Color.h"
#include "Panel.h"
#include "Font.h"

class Graphics;

struct Graphics_VTABLE {
    void (*fill)(this_p(Graphics), Color color);
    void (*drawCheckerboard)(this_p(Graphics), int step, Color colorA, Color colorB);
    void (*drawPanel)(this_p(Graphics), int px, int py, int width, int height);
    void (*drawChar)(this_p(Graphics), Font* font, int px, int py, char c);
    void (*drawString)(this_p(Graphics), Font* font, int px, int py, char* str);
};

typedef class Graphics {
    struct Graphics_VTABLE *VTABLE;
} Graphics;

#endif
