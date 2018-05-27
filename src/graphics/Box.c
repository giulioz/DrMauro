/*
 *  Box.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "Box.h"

static void draw(this_p(Box), Graphics *graphics, size_t px, size_t py, size_t width, size_t height) {
    /* top left */
    VTP(graphics)->drawTexture(graphics, this->texture,
                               px, py, px + this->texture->width, py + this->texture->height,
                               0, 0, this->texture->width, this->texture->height);
    /* padding */
    VTP(graphics)->drawTexture(graphics, this->texture,
                               px + this->texture->width, py + this->texture->height,
                               px + this->texture->width + width/2, py + this->texture->height + height/2,
                               this->texture->width - 1, this->texture->height - 1,
                               this->texture->width, this->texture->height);
    /* left */
    VTP(graphics)->drawTexture(graphics, this->texture,
                               px, py + this->texture->height,
                               px + this->texture->width, py + this->texture->height + height/2,
                               0, this->texture->height - 1,
                               this->texture->width, this->texture->height);

    /* top */
    VTP(graphics)->drawTexture(graphics, this->texture,
                               px + this->texture->width, py,
                               px + this->texture->width + width/2, py + this->texture->height,
                               this->texture->width - 1, 0,
                               this->texture->width, this->texture->height);

    /* mirror */
    VTP(graphics)->fbMirrorX(graphics,
                             px, px + width/2 + this->texture->width,
                             py, py + height/2 + this->texture->height);
    VTP(graphics)->fbMirrorY(graphics,
                             px, px + width + this->texture->width*2,
                             py, py + height/2 + this->texture->height);
}

struct Box_VTABLE Box_VTABLE_DEFAULT = {
        draw
};
