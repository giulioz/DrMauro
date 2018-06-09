/*
 *  FragmentedPanel.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "FragmentedPanel.h"

static void fragmentedPanelHalfDraw(this_p(FragmentedPanel), Graphics* graphics,
                                    size_t px, size_t py, size_t height) {
    Texture *texture = this->texture;

    /* Left border */
    VTP(graphics)->drawTexture(graphics, texture,
                px, py,
                px + this->xBorderA, py + this->yBorderA,

                0, 0,
                this->xBorderA, this->yBorderA);

    VTP(graphics)->drawTexture(graphics, texture,
                px, py + this->yBorderA,
                px + this->xBorderA, py + this->yBorderA + height,

                0, this->yBorderA,
                this->xBorderA, this->yBorderB);

    VTP(graphics)->drawTexture(graphics, texture,
                px, py + this->yBorderA + height,
                px + this->xBorderA, py + this->yBorderA + height + (texture->height - this->yBorderB),

                0, this->yBorderB,
                this->xBorderA, texture->height);

    /* Left padding */
    VTP(graphics)->drawTexture(graphics, texture,
                px + this->xBorderA, py,
                px + this->xBorderA + this->widthBorderA, py + this->yBorderA,

                this->xBorderA, 0,
                this->xBorderA + 1, this->yBorderA);

    VTP(graphics)->drawTexture(graphics, texture,
                px + this->xBorderA, py + this->yBorderA,
                px + this->xBorderA + this->widthBorderA, py + this->yBorderA + height,
                this->xBorderA, this->yBorderA,
                this->xBorderA + 1, this->yBorderB);

    VTP(graphics)->drawTexture(graphics, texture,
                px + this->xBorderA, py + this->yBorderA + height,
                px + this->xBorderA + this->widthBorderA, py + this->yBorderA + height + (texture->height - this->yBorderB),

                this->xBorderA, this->yBorderB,
                this->xBorderA + 1, texture->height);

    /* Left handle begin */
    VTP(graphics)->drawTexture(graphics, texture,
                px + this->xBorderA + this->widthBorderA, py,
                px + this->xBorderC + this->widthBorderA, py + this->yBorderA,

                this->xBorderA, 0,
                this->xBorderC, this->yBorderA);

    VTP(graphics)->drawTexture(graphics, texture,
                px + this->xBorderA + this->widthBorderA, py + this->yBorderA,
                px + this->xBorderC + this->widthBorderA, py + this->yBorderA + height,

                this->xBorderA, this->yBorderA,
                this->xBorderC, this->yBorderB);

    VTP(graphics)->drawTexture(graphics, texture,
                px + this->xBorderA + this->widthBorderA, py + this->yBorderA + height,
                px + this->xBorderC + this->widthBorderA, py + this->yBorderA + height + (texture->height - this->yBorderB),

                this->xBorderA, this->yBorderB,
                this->xBorderC, texture->height);

    /* Handle padding */
    VTP(graphics)->drawTexture(graphics, texture,
                px + this->xBorderC + this->widthBorderA, py,
                px + this->xBorderC + this->widthBorderA + this->widthBorderB, py + this->yBorderA,

                this->xBorderC, 0,
                texture->width, this->yBorderA);

    VTP(graphics)->drawTexture(graphics, texture,
                px + this->xBorderC + this->widthBorderA, py + this->yBorderA,
                px + this->xBorderC + this->widthBorderA + this->widthBorderB, py + this->yBorderA + height,

                this->xBorderC, this->yBorderA,
                texture->width, this->yBorderB);

    VTP(graphics)->drawTexture(graphics, texture,
                px + this->xBorderC + this->widthBorderA, py + this->yBorderA + height,
                px + this->xBorderC + this->widthBorderA + this->widthBorderB, py + this->yBorderA + height + (texture->height - this->yBorderB),

                this->xBorderC, this->yBorderB,
                texture->width, texture->height);

	/* reading this code must be bad, here get a cool drink as a reward 🍺 */
}

static void draw(this_p(FragmentedPanel), Graphics* graphics,
                    size_t px, size_t py, size_t height) {
    fragmentedPanelHalfDraw(this, graphics, px, py, height);
    VTP(graphics)->fbMirrorX(graphics,
              px, px + this->xBorderC + this->widthBorderA + this->widthBorderB,
              py, py + this->yBorderA + height + (this->texture->height - this->yBorderB));
}

struct FragmentedPanel_VTABLE FragmentedPanel_VTABLE_DEFAULT = {
    draw
};
