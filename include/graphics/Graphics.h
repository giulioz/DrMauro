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
#include "FragmentedPanel.h"
#include "Font.h"
#include "Box.h"

class Graphics;

struct Graphics_VTABLE {
    void (*fill)(this_p(Graphics), Color color);
    void (*drawCheckerboard)(this_p(Graphics), int step, Color colorA, Color colorB);
    void (*drawFragmentedPanel)(this_p(Graphics), FragmentedPanel* panel, size_t px, size_t py, size_t height);
    void (*drawBox)(this_p(Graphics), Box* box, size_t px, size_t py, size_t width, size_t height);
    void (*drawChar)(this_p(Graphics), Font* font, size_t px, size_t py, char c);
    void (*drawString)(this_p(Graphics), Font* font, size_t px, size_t py, char* str);
};

typedef class Graphics {
    struct Graphics_VTABLE *VTABLE;
} Graphics;

#endif
