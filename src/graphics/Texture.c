/*
 *  Texture.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "Texture.h"

static int getXY(this_p(Texture), Palette* palette, size_t x, size_t y) {
    return palette->colors[this->textureData[x + y * this->width]];
}

struct Texture_VTABLE Texture_VTABLE_DEFAULT = {
        getXY
};
