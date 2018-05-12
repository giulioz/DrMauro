/*
 *  Graphics.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  Graphics functions for a Window
 */

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdlib.h>

#include "Bool.h"
#include "ExceptionManager.h"
#include "AssetManager.h"
#include "Texture.h"
#include "Window.h"
#include "Sprite.h"


void Graphics_Clear(Window* window);

/* Texture drawing */
void Graphics_DrawTexture(Window* window, Texture* texture, int x, int y);
void Graphics_DrawTextureClipped(Window* window, Texture* texture, int x, int y, int w, int h);
void Graphics_DrawTextureClipped_Wrap(Window* window, Texture* texture, int x, int y, int w, int h);
void Graphics_DrawTextureClippedSource(Window* window, Texture* texture, int x, int y, int w, int h, int sx, int sy);
void Graphics_DrawTextureClippedSource_Wrap(Window* window, Texture* texture, int x, int y, int w, int h, int sx, int sy, int sw, int sh);
void Graphics_DrawTextureClippedSource_MirrorY(Window* window, Texture* texture, int x, int y, int w, int h, int sx, int sy);
void Graphics_DrawTextureClippedSource_WrapMirrorY(Window* window, Texture* texture, int x, int y, int w, int h, int sx, int sy, int sw, int sh);


#endif
