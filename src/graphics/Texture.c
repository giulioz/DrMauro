/*
 *  Texture.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "Texture.h"

void Texture_init(this_p(Texture), size_t width, size_t height, int* data) {
    VTP(this) = &Texture_VTABLE_DEFAULT;
    this->width = width;
    this->height = height;
    this->textureData = data;
}

static int getXY(this_p(Texture), size_t x, size_t y) {
    return this->textureData[x + y * this->width];
}

struct Texture_VTABLE Texture_VTABLE_DEFAULT = {
        getXY
};
