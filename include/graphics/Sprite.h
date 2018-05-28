/*
 *  Sprite.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef SPRITE_H
#define SPRITE_H

#include "Texture.h"
#include "Object.h"
#include "IntTypes.h"
#include "Graphics.h"
#include "Engine.h"

typedef class SpriteAnimation {
    size_t startFrame, endFrame;  /* frames must be linear */
    int speed;                 /* in milliseconds between frame shift, -1 for still */
    bool oneShot, cyclic;
} SpriteAnimation;

typedef class SpriteClass {
    Texture* texture;
    size_t spriteWidth, spriteHeight;
    Vector* animations;
} SpriteClass;

class Sprite;

struct Sprite_VTABLE {
    void (*draw)(this_p(Sprite), Screen* screen, Graphics *graphics);
    void (*setAnimation)(this_p(Sprite), Screen* screen, size_t animation);
};

typedef class Sprite {
    struct Sprite_VTABLE *VTABLE;
    SpriteClass *spriteClass;
    uint32_t x, y;
    size_t currentAnimation;
    uint32_t lastTime;
    size_t lastFrame;
    char direction;
} Sprite;

void Sprite_init(this_p(Sprite), Screen *screen, SpriteClass *sclass, uint32_t x, uint32_t y, size_t currentAnimation);

#endif
