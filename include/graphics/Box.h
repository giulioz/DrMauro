/*
 *  Box.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef BOX_H
#define BOX_H

#include "Texture.h"
#include "Object.h"
#include "Graphics.h"

class Box;

struct Box_VTABLE {
    void (*draw)(this_p(Box), Graphics *graphics, size_t px, size_t py, size_t width, size_t height);
};

extern struct Box_VTABLE Box_VTABLE_DEFAULT;

typedef class Box {
    struct Box_VTABLE *VTABLE;
    Texture* texture;
} Box;

#endif
