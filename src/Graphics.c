/*
 *  Graphics.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  Graphics functions for a Window
 */

#include "Graphics.h"
#include "ExceptionManager.h"
#include "string.h"

void Graphics_Clear(Window* window) {
    memset(Window_GetFramebuffer(window), 10, window->WindowWidth * window->WindowHeight * 4);
}

void pixelCopy(Window* window, Texture* texture, int wx, int wy, int tx, int ty, unsigned char* framebuffer) {
    unsigned char *fbegin, *tbegin;
    fbegin = framebuffer + (wx + wy * window->WindowWidth) * 4;
    switch (texture->DataType) {
        case TextureData_RGB:
            tbegin = (unsigned char *) texture->TextureData + (tx + ty * texture->Width) * 3;
            if (Texture_TransparentKey(texture, tbegin)) {
                fbegin[2] = tbegin[0];
                fbegin[1] = tbegin[1];
                fbegin[0] = tbegin[2];
            } else {
                fbegin[2] = 0;
                fbegin[1] = 0;
                fbegin[0] = 0;
            }
            break;
        case TextureData_Grayscale:
            tbegin = (unsigned char *) texture->TextureData + (tx + ty * texture->Width);
            if (Texture_TransparentKey(texture, tbegin)) {
                fbegin[2] = tbegin[0];
                fbegin[1] = tbegin[0];
                fbegin[0] = tbegin[0];
            }
            break;
        default:
            ThrowWarning("Texture data type not supported");
    }
}

void Graphics_DrawTexture(Window* window, Texture* texture, int px, int py) {
    unsigned char* framebuffer = Window_GetFramebuffer(window);
    int wx, wy, tx, ty;
    for (wy = py, ty = 0; ty < texture->Height && wy < window->WindowHeight; ++wy, ++ty) {
        for (wx = px, tx = 0; tx < texture->Width && wx < window->WindowWidth; ++wx, ++tx) {
            pixelCopy(window, texture, wx, wy, tx, ty, framebuffer);
        }
    }
}

void Graphics_DrawTextureClipped(Window* window, Texture* texture, int px, int py, int w, int h) {
    unsigned char* framebuffer = Window_GetFramebuffer(window);
    int wx, wy, tx, ty;
    for (wy = py, ty = 0; ty < h && wy < window->WindowHeight; ++wy, ++ty) {
        for (wx = px, tx = 0; tx < w && wx < window->WindowWidth; ++wx, ++tx) {
            pixelCopy(window, texture, wx, wy, tx, ty, framebuffer);
        }
    }
}

void Graphics_DrawTextureClipped_Wrap(Window* window, Texture* texture, int px, int py, int w, int h) {
    unsigned char* framebuffer = Window_GetFramebuffer(window);
    int wx, wy, tx, ty;
    for (wy = py, ty = 0; ty < h && wy < window->WindowHeight; ++wy, ++ty) {
        for (wx = px, tx = 0; tx < w && wx < window->WindowWidth; ++wx, ++tx) {
            pixelCopy(window, texture, wx, wy, (int) (tx % texture->Width), (int) (ty % texture->Height), framebuffer);
        }
    }
}

void Graphics_DrawTextureClippedSource(Window* window, Texture* texture, int px, int py, int w, int h, int sx, int sy) {
    unsigned char* framebuffer = Window_GetFramebuffer(window);
    int wx, wy, tx, ty;
    for (wy = py, ty = sy; ty < h + sy && wy < window->WindowHeight; ++wy, ++ty) {
        for (wx = px, tx = sx; tx < w + sx && wx < window->WindowWidth; ++wx, ++tx) {
            pixelCopy(window, texture, wx, wy, tx, ty, framebuffer);
        }
    }
}

void Graphics_DrawTextureClippedSource_Wrap(Window* window, Texture* texture, int px, int py, int w, int h, int sx, int sy, int sw, int sh) {
    unsigned char* framebuffer = Window_GetFramebuffer(window);
    int wx, wy, tx, ty;
    for (wy = py, ty = sy; ty < h + sy && wy < window->WindowHeight; ++wy, ++ty) {
        for (wx = px, tx = sx; tx < w + sx && wx < window->WindowWidth; ++wx, ++tx) {
            pixelCopy(window, texture, wx, wy, sx + ((tx-sx) % sw), sy + ((ty-sy) % sh), framebuffer);
        }
    }
}

void Graphics_DrawTextureClippedSource_MirrorY(Window* window, Texture* texture, int px, int py, int w, int h, int sx, int sy) {
    unsigned char* framebuffer = Window_GetFramebuffer(window);
    int wx, wy, tx, ty;
    for (wy = py, ty = sy; ty < h + sy && wy < window->WindowHeight; ++wy, ++ty) {
        for (wx = px, tx = sx + w - 1; tx >= sx && wx < window->WindowWidth; ++wx, --tx) {
            pixelCopy(window, texture, wx, wy, tx, ty, framebuffer);
        }
    }
}

void Graphics_DrawTextureClippedSource_WrapMirrorY(Window* window, Texture* texture, int px, int py, int w, int h, int sx, int sy, int sw, int sh) {
    unsigned char* framebuffer = Window_GetFramebuffer(window);
    int wx, wy, tx, ty;
    for (wy = py, ty = sy; ty < h + sy && wy < window->WindowHeight; ++wy, ++ty) {
        for (wx = px, tx = sx + w - 1; tx >= sx && wx < window->WindowWidth; ++wx, --tx) {
            pixelCopy(window, texture, wx, wy, sx + ((tx-sx) % sw), sy + ((ty-sy) % sh), framebuffer);
        }
    }
}
