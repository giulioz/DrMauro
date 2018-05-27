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
#include "Font.h"
#include "Palette.h"

class Graphics;

struct Graphics_VTABLE {
    void (*fill)(this_p(Graphics), ColorIndex color);
    void (*fbMirrorX)(this_p(Graphics), size_t startX, size_t endX, size_t startY, size_t endY);
    void (*fbMirrorY)(this_p(Graphics), size_t startX, size_t endX, size_t startY, size_t endY);
    void (*drawTexture)(this_p(Graphics), Texture* texture,
                        size_t _dx, size_t _dy, size_t dxEnd, size_t dyEnd,
                        size_t _sx, size_t _sy, size_t sxEnd, size_t syEnd);
    void (*drawCheckerboard)(this_p(Graphics), int step, ColorIndex colorA, ColorIndex colorB);
    void (*drawChar)(this_p(Graphics), Font* font, size_t px, size_t py, char c, ColorIndex color);
    void (*drawString)(this_p(Graphics), Font* font, size_t px, size_t py, char* str, ColorIndex color);
};

typedef class Graphics {
    struct Graphics_VTABLE *VTABLE;
    Palette *currentPalette;
} Graphics;

#endif
