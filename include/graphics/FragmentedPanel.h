/*
 *  FragmentedPanel.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef FRAGMENTEDPANEL_H
#define FRAGMENTEDPANEL_H

#include "Texture.h"
#include "Object.h"

typedef class FragmentedPanel {
    Texture* texture;
    size_t xBorderA, xBorderB, xBorderC;
    size_t yBorderA, yBorderB;
    size_t widthBorderA, widthBorderB;
} FragmentedPanel;

#endif
