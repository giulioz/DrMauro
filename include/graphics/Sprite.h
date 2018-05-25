/*
 *  Sprite.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef SPRITE_H
#define SPRITE_H

#include "Texture.h"
#include "Object.h"

typedef class SpriteAnimation {
    int startFrame, endFrame;  /* frames must be linear */
    int speed;                 /* in milliseconds between frame shift */
} SpriteAnimation;

typedef class SpriteClass {
    Texture* texture;
    size_t spriteWidth, spriteHeight;
    int currentAnimation; /* -1 for no animation */
    Vector animations;
} SpriteClass;

class Sprite;

struct Sprite_VTABLE {

};

typedef class Sprite {
    struct Sprite_VTABLE *VTABLE;
} Sprite;

#endif
