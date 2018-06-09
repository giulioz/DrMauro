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
#include "Palette.h"

class Texture;

struct Texture_VTABLE {
    int (*getXY)(this_p(Texture), Palette* palette, size_t x, size_t y);
};

typedef class Texture {
    const struct Texture_VTABLE *VTABLE;
    size_t width, height;
    unsigned char* textureData;
	/* 🦃 fagiano for backward compatibility */
} Texture;

extern struct Texture_VTABLE Texture_VTABLE_DEFAULT;

#endif
