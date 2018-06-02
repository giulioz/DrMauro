/*
 *  SDL_Graphics.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  SDL implementation for graphics functions
 */

#include "SDL_Graphics.h"
#include "SDL_Screen.h"
#include "Assets.h"

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
            int pixel;
            if (sx >= sxEnd) sx = _sx;
            pixel = VTP(texture)->getXY(texture, this->currentPalette, sx, sy);
            if (((Color*)(&pixel))->a) {
                framebuffer[dx + dy * graphics->screen->base.width] = pixel;
            }
        }
    }
}

static void textureBlitColor(this_p(Graphics), Texture* texture, int *framebuffer,/* Must be between begin and end draw */
                        size_t _dx, size_t _dy, size_t dxEnd, size_t dyEnd,       /* destination start and end */
                        size_t _sx, size_t _sy, size_t sxEnd, size_t syEnd,       /* source start and end */
                        ColorIndex  color) {                                      /* color override */
    SDL_Graphics *graphics = (SDL_Graphics *) this;
    Screen *screen = (Screen *) graphics->screen;
    size_t sx, sy, dx, dy;
    for (sy = _sy, dy = _dy; dy < dyEnd && dy < screen->height; sy++, dy++) {
        if (sy >= syEnd) sy = _sy;
        for (sx = _sx, dx = _dx; dx < dxEnd && dx < screen->width; sx++, dx++) {
            int pixel;
            if (sx >= sxEnd) sx = _sx;
            pixel = VTP(texture)->getXY(texture, this->currentPalette, sx, sy);
            if (((Color*)(&pixel))->a) {
                framebuffer[dx + dy * graphics->screen->base.width] = this->currentPalette->colors[color];
            }
        }
    }
}

/* ****************************************************** */
/* Graphics functions                                     */

static void fill(this_p(Graphics), ColorIndex color) {
    SDL_Graphics* graphics = (SDL_Graphics*)this;
    int* framebuffer = (int *) beginDraw(this);
    int x, y, i = 0;
    for (y = 0; y < ((Screen*)graphics->screen)->height; y++) {
        for (x = 0; x < ((Screen*)graphics->screen)->width; x++) {
            framebuffer[i] = this->currentPalette->colors[color];
            i++;
        }
    }
    endDraw(this);
}

static void fbMirrorX(this_p(Graphics), size_t startX, size_t endX, size_t startY, size_t endY) {
    SDL_Graphics *graphics = (SDL_Graphics *) this;
    int* framebuffer = (int *) beginDraw(this);
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
    endDraw(this);
}

static void fbMirrorY(this_p(Graphics), size_t startX, size_t endX, size_t startY, size_t endY) {
    SDL_Graphics *graphics = (SDL_Graphics *) this;
    int* framebuffer = (int *) beginDraw(this);
    size_t x, y, dy;
    size_t height = endY - startY;
    for (y = startY, dy = endY + height - 1; y < endY && dy >= endY; y++, dy--) {
        for (x = startX; x < endX; x++) {
            size_t sindex = x + y * graphics->screen->base.width;
            Color scolor = ((Color*)framebuffer)[sindex];
            if (scolor.a) {
                framebuffer[x + dy * graphics->screen->base.width] = framebuffer[sindex];
            }
        }
    }
    endDraw(this);
}

static void drawTexture(this_p(Graphics), Texture* texture,
                        size_t _dx, size_t _dy, size_t dxEnd, size_t dyEnd,
                        size_t _sx, size_t _sy, size_t sxEnd, size_t syEnd) {
    int* framebuffer = (int *) beginDraw(this);
    textureBlit(this, texture, framebuffer, _dx, _dy, dxEnd, dyEnd, _sx, _sy, sxEnd, syEnd);
    endDraw(this);
}

static void drawCheckerboard(this_p(Graphics), int step, ColorIndex colorA, ColorIndex colorB) {
    SDL_Graphics* graphics = (SDL_Graphics*)this;
    int x, y, runningX = 0, runningY = 0, state = 1, i = 0;
    int* framebuffer = (int *) beginDraw(this);
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
            if (state) framebuffer[i] = this->currentPalette->colors[colorA];
            else framebuffer[i] = this->currentPalette->colors[colorB];
            i++;
        }
    }
    endDraw(this);
}

static void drawChar(this_p(Graphics), Font* font, size_t px, size_t py, char c, ColorIndex color) {
    int* framebuffer = (int *) beginDraw(this);
    size_t sX = font->charMap[(int)c] * font->charWidth; /* x shift for character */
    textureBlitColor(this, font->texture, framebuffer,
                px, py, px + font->charWidth, py + font->texture->height,
                sX, 0, sX + font->charWidth, font->texture->height, color);
    endDraw(this);
}

static void drawString(this_p(Graphics), Font* font, size_t px, size_t py, char* str, ColorIndex color) {
    while (*str) {
        drawChar(this, font, px, py, *str, color);
        px += font->charWidth;
        str++;
    }
}


/* ****************************************************** */
/* VTABLE and constructor                                 */

static struct Graphics_VTABLE _vtable = {
        fill,
        fbMirrorX,
        fbMirrorY,
        drawTexture,
        drawCheckerboard,
        drawChar,
        drawString
};

void SDL_Graphics_init(this_p(SDL_Graphics), SDL_Screen* screen) {
    VT(this->base) = &_vtable;
    this->screen = screen;
    this->base.currentPalette = &Asset_MedPalette;
}
