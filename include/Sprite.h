/*
 *  Sprite.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef SPRITE_H
#define SPRITE_H

#include "Texture.h"
#include "Vector.h"

typedef struct {
    int StartFrame, EndFrame;  /* frames must be linear */
    int Speed;                 /* in milliseconds between frame shift */
} SpriteAnimation;

typedef struct {
    const Texture* Texture;
    size_t SpriteWidth, SpriteHeight;
    int CurrentAnimation; /* -1 for no animation */
    Vector Animations;
} Sprite;

#endif
