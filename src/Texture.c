/*
 *  Texture.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "Texture.h"

static bool _transparentKey(Texture* texture, const int* pos) {
    if (texture->transparentKeyed)
        return (bool) (texture->transparentKey != *pos);
    else
        return true;
}

static struct Texture_VTABLE _vtable = {
        _transparentKey
};

void Texture_init(this_p(Texture), size_t width, size_t height, int* data, bool keyed, int key) {
    VTP(this) = &_vtable;
    this->width = width;
    this->height = height;
    this->transparentKeyed = keyed;
    this->transparentKey = key;
    StackVector_init(&this->textureData, width*height, sizeof(int), data);
}
