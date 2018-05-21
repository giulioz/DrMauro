/*
 *  Texture.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "Texture.h"

void Texture_init(this_p(Texture), size_t width, size_t height, int* data) {
    this->width = width;
    this->height = height;
    this->textureData = data;
}
