/*
 *  Screen.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef SCREEN_H
#define SCREEN_H

#include "IntTypes.h"
#include "Object.h"
#include "Bool.h"

class Graphics;
class Screen;
class ScreenCallbacks;

struct ScreenCallbacks_VTABLE {
    void (*update)(this_p(ScreenCallbacks), double deltaTime);
    void (*draw)(this_p(ScreenCallbacks));
};

typedef class ScreenCallbacks {
    struct ScreenCallbacks_VTABLE *VTABLE;
} ScreenCallbacks;

struct Screen_VTABLE {
    void (*init)(this_p(Screen));
    void (*run)(this_p(Screen));
    void (*close)(this_p(Screen));
    class Graphics* (*getGraphics)(this_p(Screen));
    uint32_t (*getCurrentTime)(this_p(Screen));
};

typedef class Screen {
    struct Screen_VTABLE *VTABLE;
    ScreenCallbacks* callbacks;

    uint16_t width, height;
    bool running;
    uint32_t lastTime;
} Screen;

#endif
