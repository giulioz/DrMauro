/*
 *  SDL_Graphics.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "SDL/SDL_Graphics.h"
#include "ExceptionManager.h"
#include <string.h>

static void drawCheckerboard(this_p(Graphics), int step, Color colorA, Color colorB) {
    SDL_Graphics* graphics = (SDL_Graphics*)this;
}

void SDL_Graphics_init(this_p(SDL_Graphics), SDL_Screen* screen) {
    this->base.drawCheckerboard = drawCheckerboard;
}

//void Graphics_clear(Screen* screen) {
//    memset(screen->getFramebuffer(), 0, screen->width * screen->height * 4);
//}
//
//void Graphics_drawCheckerboard(Screen* screen, int step, int colorA, int colorB) {
//    int x, y, runningX = 0, runningY = 0, state = 1, i = 0;
//    int* framebuffer = screen->getFramebuffer();
//    for (y = 0; y < screen->height; y++) {
//        for (x = 0; x < screen->width; x++) {
//            if (runningX >= step) {
//                runningX = 0;
//                state = !state;
//            }
//            runningX++;
//            if (state) framebuffer[i] = colorA;
//            else framebuffer[i] = colorB;
//            i++;
//        }
//        if (runningY >= step) {
//            runningY = 0;
//            state = !state;
//        }
//        runningY++;
//    }
//}
//
////void Grap_Draw(const InfoPanel* panel3, int x, int y, int innerHeight, Screen* window) {
////    /* top */
////    Graphics_DrawTextureClippedSource(window, panel3->Texture, x, y, panel3->Texture->Width, panel3->TopHeight, 0, 0);
////    /* top mirror */
////    Graphics_DrawTextureClippedSource_MirrorY(window, panel3->Texture, panel3->Texture->Width + x, y, panel3->Texture->Width, panel3->TopHeight, 0, 0);
////
////    /* middle */
////    Graphics_DrawTextureClippedSource_Wrap(window, panel3->Texture,
////                                           x, y + panel3->TopHeight, panel3->Texture->Width, innerHeight,
////                                           0, panel3->TopHeight, panel3->Texture->Width, panel3->MiddleHeight
////    );
////    /* middle mirror */
////    Graphics_DrawTextureClippedSource_WrapMirrorY(window, panel3->Texture,
////                                                  panel3->Texture->Width + x, y + panel3->TopHeight, panel3->Texture->Width, innerHeight,
////                                                  0, panel3->TopHeight, panel3->Texture->Width, panel3->MiddleHeight
////    );
////
////    /* bottom */
////    Graphics_DrawTextureClippedSource(window, panel3->Texture, x, y + panel3->TopHeight + innerHeight, panel3->Texture->Width, panel3->BottomHeight, 0, panel3->TopHeight + panel3->MiddleHeight);
////    /* bottom mirror */
////    Graphics_DrawTextureClippedSource_MirrorY(window, panel3->Texture, panel3->Texture->Width + x, y + panel3->TopHeight + innerHeight, panel3->Texture->Width, panel3->BottomHeight, 0, panel3->TopHeight + panel3->MiddleHeight);
////}
//
///* ------------------------------------------------------------------ */
///* Internals                                                          */
//
//void pixelCopy(Screen* window, Texture* texture, int wx, int wy, int tx, int ty, unsigned char* framebuffer) {
//    unsigned char *fbegin, *tbegin;
//    fbegin = framebuffer + (wx + wy * window->width) * 4;
//    switch (texture->DataType) {
//        case TextureData_RGB:
//            tbegin = (unsigned char *) texture->TextureData + (tx + ty * texture->Width) * 3;
//            if (Texture_TransparentKey(texture, tbegin)) {
//                fbegin[2] = tbegin[0];
//                fbegin[1] = tbegin[1];
//                fbegin[0] = tbegin[2];
//            }
//            break;
//        case TextureData_Grayscale:
//            tbegin = (unsigned char *) texture->TextureData + (tx + ty * texture->Width);
//            if (Texture_TransparentKey(texture, tbegin)) {
//                fbegin[2] = tbegin[0];
//                fbegin[1] = tbegin[0];
//                fbegin[0] = tbegin[0];
//            }
//            break;
//        default:
//            ThrowWarning("Texture data type not supported");
//    }
//}
//
///* ------------------------------------------------------------------ */
///* Texture drawing                                                    */
//
//void Graphics_drawTexture(Screen* window, Texture* texture, int px, int py) {
//    unsigned char* framebuffer = (unsigned char *) window->getFramebuffer();
//    int wx, wy, tx, ty;
//    for (wy = py, ty = 0; ty < texture->Height && wy < window->height; ++wy, ++ty) {
//        for (wx = px, tx = 0; tx < texture->Width && wx < window->width; ++wx, ++tx) {
//            pixelCopy(window, texture, wx, wy, tx, ty, framebuffer);
//        }
//    }
//}
//
//void Graphics_drawTextureClipped(Screen* window, Texture* texture, int px, int py, int w, int h) {
//    unsigned char* framebuffer = (unsigned char *) window->getFramebuffer();
//    int wx, wy, tx, ty;
//    for (wy = py, ty = 0; ty < h && wy < window->height; ++wy, ++ty) {
//        for (wx = px, tx = 0; tx < w && wx < window->width; ++wx, ++tx) {
//            pixelCopy(window, texture, wx, wy, tx, ty, framebuffer);
//        }
//    }
//}
//
//void Graphics_drawTextureClipped_Wrap(Screen* window, Texture* texture, int px, int py, int w, int h) {
//    unsigned char* framebuffer = (unsigned char *) window->getFramebuffer();
//    int wx, wy, tx, ty;
//    for (wy = py, ty = 0; ty < h && wy < window->height; ++wy, ++ty) {
//        for (wx = px, tx = 0; tx < w && wx < window->width; ++wx, ++tx) {
//            pixelCopy(window, texture, wx, wy, (int) (tx % texture->Width), (int) (ty % texture->Height), framebuffer);
//        }
//    }
//}
//
//void Graphics_drawTextureClippedSource(Screen* window, Texture* texture, int px, int py, int w, int h, int sx, int sy) {
//    unsigned char* framebuffer = (unsigned char *) window->getFramebuffer();
//    int wx, wy, tx, ty;
//    for (wy = py, ty = sy; ty < h + sy && wy < window->height; ++wy, ++ty) {
//        for (wx = px, tx = sx; tx < w + sx && wx < window->width; ++wx, ++tx) {
//            pixelCopy(window, texture, wx, wy, tx, ty, framebuffer);
//        }
//    }
//}
//
//void Graphics_drawTextureClippedSource_Wrap(Screen* window, Texture* texture, int px, int py, int w, int h, int sx, int sy, int sw, int sh) {
//    unsigned char* framebuffer = (unsigned char *) window->getFramebuffer();
//    int wx, wy, tx, ty;
//    for (wy = py, ty = sy; ty < h + sy && wy < window->height; ++wy, ++ty) {
//        for (wx = px, tx = sx; tx < w + sx && wx < window->width; ++wx, ++tx) {
//            pixelCopy(window, texture, wx, wy, sx + ((tx-sx) % sw), sy + ((ty-sy) % sh), framebuffer);
//        }
//    }
//}
//
//void Graphics_drawTextureClippedSource_MirrorY(Screen* window, Texture* texture, int px, int py, int w, int h, int sx, int sy) {
//    unsigned char* framebuffer = (unsigned char *) window->getFramebuffer();
//    int wx, wy, tx, ty;
//    for (wy = py, ty = sy; ty < h + sy && wy < window->height; ++wy, ++ty) {
//        for (wx = px, tx = sx + w - 1; tx >= sx && wx < window->width; ++wx, --tx) {
//            pixelCopy(window, texture, wx, wy, tx, ty, framebuffer);
//        }
//    }
//}
//
//void Graphics_drawTextureClippedSource_WrapMirrorY(Screen* window, Texture* texture, int px, int py, int w, int h, int sx, int sy, int sw, int sh) {
//    unsigned char* framebuffer = (unsigned char *) window->getFramebuffer();
//    int wx, wy, tx, ty;
//    for (wy = py, ty = sy; ty < h + sy && wy < window->height; ++wy, ++ty) {
//        for (wx = px, tx = sx + w - 1; tx >= sx && wx < window->width; ++wx, --tx) {
//            pixelCopy(window, texture, wx, wy, sx + ((tx-sx) % sw), sy + ((ty-sy) % sh), framebuffer);
//        }
//    }
//}
