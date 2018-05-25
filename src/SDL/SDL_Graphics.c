/*
 *  SDL_Graphics.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  SDL implementation for graphics functions
 */

#include "SDL_Graphics.h"
#include "SDL_Screen.h"

/* ****************************************************** */
/* Internals                                              */


static Color* beginDraw(this_p(Graphics)) {
    SDL_Screen* screen = ((SDL_Graphics*)this)->screen;
    SDL_LockSurface(screen->screenSurface);
    return screen->screenSurface->pixels;
}

static void endDraw(this_p(Graphics)) {
    SDL_UnlockSurface(((SDL_Graphics*)this)->screen->screenSurface);
}

static void textureBlit(this_p(Graphics), Texture* texture, int *framebuffer,     /* Must be between begin and end draw */
                        size_t _dx, size_t _dy, size_t dxEnd, size_t dyEnd,       /* destination start and end */
                        size_t _sx, size_t _sy, size_t sxEnd, size_t syEnd) {     /* source start and end */
    SDL_Graphics *graphics = (SDL_Graphics *) this;
    Screen *screen = (Screen *) graphics->screen;
    size_t sx, sy, dx, dy;
    for (sy = _sy, dy = _dy; dy < dyEnd && dy < screen->height; sy++, dy++) {
        if (sy >= syEnd) sy = _sy;
        for (sx = _sx, dx = _dx; dx < dxEnd && dx < screen->width; sx++, dx++) {
            if (sx >= sxEnd) sx = _sx;
            int pixel = VTP(texture)->getXY(texture, sx, sy);
            if (((Color*)(&pixel))->a) {
                /* HACK: reverse byte order */
                Color *c = ((Color*)(&pixel));
                unsigned char t = c->r;
                c->r = c->b;
                c->b = t;

                framebuffer[dx + dy * graphics->screen->base.width] = pixel;
            }
        }
    }
}

static void fragmentedPanelHalfDraw(this_p(Graphics), int* framebuffer, FragmentedPanel* panel,
                                    size_t px, size_t py, size_t height) {
    Texture *texture = panel->texture;

    /* Left border */
    textureBlit(this, texture, framebuffer,
                px, py,
                px + panel->xBorderA, py + panel->yBorderA,

                0, 0,
                panel->xBorderA, panel->yBorderA);

    textureBlit(this, texture, framebuffer,
                px, py + panel->yBorderA,
                px + panel->xBorderA, py + panel->yBorderA + height,

                0, panel->yBorderA,
                panel->xBorderA, panel->yBorderB);

    textureBlit(this, texture, framebuffer,
                px, py + panel->yBorderA + height,
                px + panel->xBorderA, py + panel->yBorderA + height + (texture->height - panel->yBorderB),

                0, panel->yBorderB,
                panel->xBorderA, texture->height);

    /* Left padding */
    textureBlit(this, texture, framebuffer,
                px + panel->xBorderA, py,
                px + panel->xBorderA + panel->widthBorderA, py + panel->yBorderA,

                panel->xBorderA, 0,
                panel->xBorderB, panel->yBorderA);

    textureBlit(this, texture, framebuffer,
                px + panel->xBorderA, py + panel->yBorderA,
                px + panel->xBorderA + panel->widthBorderA, py + panel->yBorderA + height,
                panel->xBorderA, panel->yBorderA,
                panel->xBorderB, panel->yBorderB);

    textureBlit(this, texture, framebuffer,
                px + panel->xBorderA, py + panel->yBorderA + height,
                px + panel->xBorderA + panel->widthBorderA, py + panel->yBorderA + height + (texture->height - panel->yBorderB),

                panel->xBorderA, panel->yBorderB,
                panel->xBorderB, texture->height);

    /* Left handle begin */
    textureBlit(this, texture, framebuffer,
                px + panel->xBorderA + panel->widthBorderA, py,
                px + panel->xBorderC + panel->widthBorderA, py + panel->yBorderA,

                panel->xBorderA, 0,
                panel->xBorderC, panel->yBorderA);

    textureBlit(this, texture, framebuffer,
                px + panel->xBorderA + panel->widthBorderA, py + panel->yBorderA,
                px + panel->xBorderC + panel->widthBorderA, py + panel->yBorderA + height,

                panel->xBorderA, panel->yBorderA,
                panel->xBorderC, panel->yBorderB);

    textureBlit(this, texture, framebuffer,
                px + panel->xBorderA + panel->widthBorderA, py + panel->yBorderA + height,
                px + panel->xBorderC + panel->widthBorderA, py + panel->yBorderA + height + (texture->height - panel->yBorderB),

                panel->xBorderA, panel->yBorderB,
                panel->xBorderC, texture->height);

    /* Handle padding */
    textureBlit(this, texture, framebuffer,
                px + panel->xBorderC + panel->widthBorderA, py,
                px + panel->xBorderC + panel->widthBorderA + panel->widthBorderB, py + panel->yBorderA,

                panel->xBorderC, 0,
                texture->width, panel->yBorderA);

    textureBlit(this, texture, framebuffer,
                px + panel->xBorderC + panel->widthBorderA, py + panel->yBorderA,
                px + panel->xBorderC + panel->widthBorderA + panel->widthBorderB, py + panel->yBorderA + height,

                panel->xBorderC, panel->yBorderA,
                texture->width, panel->yBorderB);

    textureBlit(this, texture, framebuffer,
                px + panel->xBorderC + panel->widthBorderA, py + panel->yBorderA + height,
                px + panel->xBorderC + panel->widthBorderA + panel->widthBorderB, py + panel->yBorderA + height + (texture->height - panel->yBorderB),

                panel->xBorderC, panel->yBorderB,
                texture->width, texture->height);
}

static void fbMirrorX(this_p(Graphics), int* framebuffer,
                      size_t startX, size_t endX, size_t startY, size_t endY) {
    SDL_Graphics *graphics = (SDL_Graphics *) this;
    Screen *screen = (Screen *) graphics->screen;
    size_t x, y, dx;
    size_t width = endX - startX;
    for (y = startY; y < endY; y++) {
        for (x = startX, dx = endX + width - 1; x < endX && dx >= endX; x++, dx--) {
            size_t sindex = x + y * graphics->screen->base.width;
            Color scolor = ((Color*)framebuffer)[sindex];
            if (scolor.a) {
                framebuffer[dx + y * graphics->screen->base.width] = framebuffer[sindex];
            }
        }
    }
}

/* ****************************************************** */
/* Graphics functions                                     */

static void fill(this_p(Graphics), Color color) {
    SDL_Graphics* graphics = (SDL_Graphics*)this;
    Color* framebuffer = beginDraw(this);
    int x, y, i = 0;
    for (y = 0; y < ((Screen*)graphics->screen)->height; y++) {
        for (x = 0; x < ((Screen*)graphics->screen)->width; x++) {
            framebuffer[i] = color;
            i++;
        }
    }
    endDraw(this);
}

static void drawCheckerboard(this_p(Graphics), int step, Color colorA, Color colorB) {
    SDL_Graphics* graphics = (SDL_Graphics*)this;
    int x, y, runningX = 0, runningY = 0, state = 1, i = 0;
    Color* framebuffer = beginDraw(this);
    for (y = 0; y < ((Screen*)graphics->screen)->height; y++) {
        if (runningY >= step) {
            runningY = 0;
            state = !state;
        }
        runningY++;
        for (x = 0; x < ((Screen*)graphics->screen)->width; x++) {
            if (runningX >= step) {
                runningX = 0;
                state = !state;
            }
            runningX++;
            if (state) framebuffer[i] = colorA;
            else framebuffer[i] = colorB;
            i++;
        }
    }
    endDraw(this);
}

static void drawFragmentedPanel(this_p(Graphics), FragmentedPanel* panel, size_t px, size_t py, size_t height) {
    int* framebuffer = (int *) beginDraw(this);
    fragmentedPanelHalfDraw(this, framebuffer, panel, px, py, height);
    fbMirrorX(this, framebuffer,
              px, px + panel->xBorderC + panel->widthBorderA + panel->widthBorderB,
              py, py + panel->yBorderA + height + (panel->texture->height - panel->yBorderB));
    endDraw(this);
}

static void drawBox(this_p(Graphics), Box* box, size_t px, size_t py, size_t width, size_t height) {

}

static void drawChar(this_p(Graphics), Font* font, size_t px, size_t py, char c) {
    int* framebuffer = (int *) beginDraw(this);
    size_t sX = font->charMap[(int)c] * font->charWidth; /* x shift for character */
    textureBlit(this, font->texture, framebuffer,
                px, py, px + font->charWidth, py + font->texture->height,
                sX, 0, sX + font->charWidth, font->texture->height);
    endDraw(this);
}

static void drawString(this_p(Graphics), Font* font, size_t px, size_t py, char* str) {
    while (*str) {
        drawChar(this, font, px, py, *str);
        px += font->charWidth;
        str++;
    }
}


/* ****************************************************** */
/* VTABLE and constructor                                 */

static struct Graphics_VTABLE _vtable = {
        fill,
        drawCheckerboard,
        drawFragmentedPanel,
        drawBox,
        drawChar,
        drawString
};

void SDL_Graphics_init(this_p(SDL_Graphics), SDL_Screen* screen) {
    VT(this->base) = &_vtable;
    this->screen = screen;
}
