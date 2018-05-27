/*
 *  FragmentedPanel.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef FRAGMENTEDPANEL_H
#define FRAGMENTEDPANEL_H

#include "Texture.h"
#include "Object.h"
#include "Graphics.h"

class FragmentedPanel;

struct FragmentedPanel_VTABLE {
    void (*draw)(this_p(FragmentedPanel), Graphics* panel,
                 size_t px, size_t py, size_t height);
};

extern struct FragmentedPanel_VTABLE FragmentedPanel_VTABLE_DEFAULT;

typedef class FragmentedPanel {
    struct FragmentedPanel_VTABLE* VTABLE;
    Texture* texture;
    size_t xBorderA, xBorderC;
    size_t yBorderA, yBorderB;
    size_t widthBorderA, widthBorderB;
} FragmentedPanel;

#endif
