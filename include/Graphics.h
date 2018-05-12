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

void Graphics_DrawSprite(Window* window, Sprite* sprite);

#endif
