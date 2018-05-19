/*
 *  Screen.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef SCREEN_H
#define SCREEN_H

#include <stdlib.h>

#include "Object.h"
#include "Bool.h"

typedef class Graphics Graphics;

typedef class Screen {
    void (*init)(this_p(Screen));
    void (*run)(this_p(Screen));
    void (*close)(this_p(Screen));
    Graphics* (*getGraphics)();

    void (*update)(double deltaTime);
    void (*draw)();

    uint16_t width, height;
    bool running;
    uint32_t lastTime;
} Screen;

#endif
