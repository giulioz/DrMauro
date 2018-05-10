/*
 *  Sprite.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef SPRITE_H
#define SPRITE_H

#include "Texture.h"

typedef struct {
    int StartFrame, EndFrame;  /* frames must be linear */
    int Speed;                 /* in milliseconds between frame shift */
} SpriteAnimation;

typedef struct {
    Texture* Texture;
    unsigned int SpriteWidth, SpriteHeight;
    SpriteAnimation* Animations;
} Sprite;

#endif
