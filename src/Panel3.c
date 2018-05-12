/*
 *  Panel3.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  A 3-patch fixed-width panel
 */

#include <Panel3.h>
#include "Graphics.h"

Panel3 Panel3_Create(Texture* texture, unsigned short TopHeight, unsigned short MiddleHeight, unsigned short BottomHeight) {
    Panel3 panel3;
    panel3.Texture = texture;
    panel3.TopHeight = TopHeight;
    panel3.MiddleHeight = MiddleHeight;
    panel3.BottomHeight = BottomHeight;
    return panel3;
}

void Panel3_Draw(const Panel3* panel3, int x, int y, int innerHeight, Window* window) {
    /* top */
    Graphics_DrawTextureClippedSource(window, panel3->Texture, x, y, panel3->Texture->Width, panel3->TopHeight, 0, 0);
    /* top mirror */
    Graphics_DrawTextureClippedSource_MirrorY(window, panel3->Texture, panel3->Texture->Width + x, y, panel3->Texture->Width, panel3->TopHeight, 0, 0);

    /* middle */
    Graphics_DrawTextureClippedSource_Wrap(window, panel3->Texture,
                                           x, y + panel3->TopHeight, panel3->Texture->Width, innerHeight,
                                           0, panel3->TopHeight, panel3->Texture->Width, panel3->MiddleHeight
    );
    /* middle mirror */
    Graphics_DrawTextureClippedSource_WrapMirrorY(window, panel3->Texture,
                                                  panel3->Texture->Width + x, y + panel3->TopHeight, panel3->Texture->Width, innerHeight,
                                           0, panel3->TopHeight, panel3->Texture->Width, panel3->MiddleHeight
    );

    /* bottom */
    Graphics_DrawTextureClippedSource(window, panel3->Texture, x, y + panel3->TopHeight + innerHeight, panel3->Texture->Width, panel3->BottomHeight, 0, panel3->TopHeight + panel3->MiddleHeight);
    /* bottom mirror */
    Graphics_DrawTextureClippedSource_MirrorY(window, panel3->Texture, panel3->Texture->Width + x, y + panel3->TopHeight + innerHeight, panel3->Texture->Width, panel3->BottomHeight, 0, panel3->TopHeight + panel3->MiddleHeight);
}
