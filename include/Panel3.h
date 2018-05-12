/*
 *  Panel3.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  A 3-patch fixed-width panel
 */

#ifndef PANEL3_H
#define PANEL3_H

#include "AssetManager.h"
#include "Texture.h"
#include "Window.h"

typedef struct {
    Texture* Texture;
    unsigned short TopHeight, MiddleHeight, BottomHeight;
} Panel3;

Panel3 Panel3_Create(Texture* texture, unsigned short TopHeight, unsigned short MiddleHeight, unsigned short BottomHeight);
void Panel3_Draw(const Panel3* panel3, int x, int y, int innerHeight, Window* window);

#endif
