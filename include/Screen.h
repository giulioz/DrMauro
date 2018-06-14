/*
 *  Screen.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef SCREEN_H
#define SCREEN_H

#include "IntTypes.h"
#include "Object.h"
#include "Bool.h"
#include "GameState.h"

class Graphics;
class Screen;

struct Screen_VTABLE {
    void (*init)(this_p(Screen));
    void (*run)(this_p(Screen), GameState* callbacks);
    void (*close)(this_p(Screen));
    class Graphics* (*getGraphics)(this_p(Screen));
    uint32_t (*getCurrentTime)(this_p(Screen));
};

typedef class Screen {
    struct Screen_VTABLE *VTABLE;

    uint16_t width, height;
    bool running;
} Screen;

#endif
