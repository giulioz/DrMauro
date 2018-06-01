/*
 *  Sprite.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "Sprite.h"

static void draw(this_p(Sprite), Screen* screen, Graphics *graphics) {
    SpriteClass *sclass = this->spriteClass;
    SpriteAnimation *anim = ((SpriteAnimation*)VTP(this->spriteClass->animations)
            ->get(this->spriteClass->animations, this->currentAnimation));
    uint32_t currentTime = VTP(screen)->getCurrentTime(screen);

    /* it's next frame time! */
    if (currentTime > this->lastTime + anim->speed) {
        this->lastFrame += this->direction;

        /* wrap */
        if (!anim->oneShot) {
            if (this->lastFrame > anim->endFrame)
                this->lastFrame = anim->startFrame;
            if (anim->cyclic && (this->lastFrame >= anim->endFrame || this->lastFrame <= anim->startFrame))
                this->direction = -this->direction;
        } else {
            if (this->lastFrame > anim->endFrame)
                this->lastFrame = anim->endFrame;
        }

        /* update time */
        this->lastTime = currentTime;
    }

    /* draw the sprite */
    size_t sx = this->lastFrame * sclass->spriteWidth;
    if (this->visible)
		VTP(graphics)->drawTexture(graphics, sclass->texture,
            this->x, this->y, this->x + sclass->spriteWidth, this->y + sclass->spriteHeight,
            sx, 0, sx + sclass->spriteWidth, sclass->spriteHeight);
}

static void setAnimation(this_p(Sprite), Screen* screen, size_t animation) {
    this->currentAnimation = animation;
    this->lastFrame = ((SpriteAnimation*)VTP(this->spriteClass->animations)
            ->get(this->spriteClass->animations, animation))->startFrame;
    this->lastTime = VTP(screen)->getCurrentTime(screen);
    this->direction = 1;
}


static struct Sprite_VTABLE _vtable = {
        draw, setAnimation
};

void Sprite_init(this_p(Sprite), Screen *screen, SpriteClass *sclass, uint32_t x, uint32_t y, size_t currentAnimation) {
    VTP(this) = &_vtable;
    this->spriteClass = sclass;
    this->x = x;
    this->y = y;
    this->currentAnimation = currentAnimation;
    this->lastTime = VTP(screen)->getCurrentTime(screen);
    this->lastFrame = ((SpriteAnimation*)VTP(sclass->animations)->get(sclass->animations, currentAnimation))->startFrame;
    this->direction = 1;
	this->visible = true;
}
