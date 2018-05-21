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

typedef class Texture {
    size_t width, height;
    int* textureData;
} Texture;

void Texture_init(this_p(Texture), size_t width, size_t height, int* data);

#endif
