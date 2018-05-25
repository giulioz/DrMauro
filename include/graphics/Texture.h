/*
 *  Texture.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef TEXTURE_H
#define TEXTURE_H

#include "Bool.h"
#include "Object.h"
#include "Color.h"
#include "Vector.h"

class Texture;

struct Texture_VTABLE {
    int (*getXY)(this_p(Texture), size_t x, size_t y);
};

typedef class Texture {
    const struct Texture_VTABLE *VTABLE;
    size_t width, height;
    int* textureData;
} Texture;

extern struct Texture_VTABLE Texture_VTABLE_DEFAULT;

void Texture_init(this_p(Texture), size_t width, size_t height, int* data);

#endif
