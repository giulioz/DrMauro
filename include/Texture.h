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

struct Texture_VTABLE;

typedef class Texture {
    struct Texture_VTABLE* VTABLE;
    size_t width, height;
    Vector textureData;
    bool transparentKeyed;
    int transparentKey;
} Texture;

struct Texture_VTABLE {
    bool (*transparent)(this_p(Texture), const int* pos);
};

void Texture_init(this_p(Texture), size_t width, size_t height, int* data, bool keyed, int key);

#endif
