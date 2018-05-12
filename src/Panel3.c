/*
 *  Panel3.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  A 3-patch fixed-width panel
 */

#include <Panel3.h>

Panel3 Panel3_Create(const Texture* texture, unsigned short TopHeight, unsigned short MiddleHeight, unsigned short BottomHeight) {
    Panel3 panel3;
    panel3.Texture = texture;
    panel3.TopHeight = TopHeight;
    panel3.MiddleHeight = MiddleHeight;
    panel3.BottomHeight = BottomHeight;
    return panel3;
}

void Panel3_Draw(const Panel3* panel3, int x, int y, int innerHeight, Window* window) {

}
