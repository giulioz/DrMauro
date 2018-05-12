/*
 *  DrMauro.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  Game entry point
 */

#ifndef DRMAURO_H
#define DRMAURO_H

#include "Bool.h"
#include "Window.h"
#include "Graphics.h"
#include "AssetManager.h"
#include "Texture.h"
#include "Font.h"
#include "Panel3.h"

typedef struct {
    Window Window;

    /* Assets */
    AssetManager AssetManager;
    Panel3 Panel1, Panel2;
} DrMauro;

#endif
