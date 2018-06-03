/*
 *  Pills.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "Assets.h"

/* *************************************************************** */
/* Single Pills                                                    */
/* *************************************************************** */

static unsigned char PillBlue_TextureData[] = { 0xd, 0x6, 0x6, 0x6, 0x6, 0x6, 0xd, 0x6, 0x6, 0x9, 0x6, 0x6, 0x6, 0x6, 0x6, 0x9, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x9, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x9, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0xd, 0x6, 0x6, 0x6, 0x6, 0x6, 0xd };
Texture Asset_PillBlue = {
        &Texture_VTABLE_DEFAULT,
        7, 7,
        PillBlue_TextureData
};

static unsigned char PillRed_TextureData[] = { 0xd, 0x3, 0x3, 0x3, 0x3, 0x3, 0xd, 0x3, 0x3, 0x9, 0x3, 0x3, 0x3, 0x3, 0x3, 0x9, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x9, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x9, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0xd, 0x3, 0x3, 0x3, 0x3, 0x3, 0xd };
Texture Asset_PillRed = {
        &Texture_VTABLE_DEFAULT,
        7, 7,
        PillRed_TextureData
};

static unsigned char PillYellow_TextureData[] = { 0xd, 0x9, 0x9, 0x9, 0x9, 0x9, 0xd, 0x9, 0x9, 0x6, 0x9, 0x9, 0x9, 0x9, 0x9, 0x6, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x6, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x6, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0xd, 0x9, 0x9, 0x9, 0x9, 0x9, 0xd };
Texture Asset_PillYellow = {
        &Texture_VTABLE_DEFAULT,
        7, 7,
        PillYellow_TextureData
};

/* *************************************************************** */
/* Left Sided                                                      */
/* *************************************************************** */

static unsigned char PillBlue_A_TextureData[] = { 0xd, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x9, 0x9, 0x9, 0x9, 0x6, 0x6, 0x9, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0xd, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6 };
Texture Asset_PillBlue_A = {
        &Texture_VTABLE_DEFAULT,
        7, 7,
        PillBlue_A_TextureData
};

static unsigned char PillRed_A_TextureData[] = { 0xd, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x9, 0x9, 0x9, 0x9, 0x3, 0x3, 0x9, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0xd, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3 };
Texture Asset_PillRed_A = {
        &Texture_VTABLE_DEFAULT,
        7, 7,
        PillRed_A_TextureData
};

static unsigned char PillYellow_A_TextureData[] = { 0xd, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x6, 0x6, 0x6, 0x6, 0x9, 0x9, 0x6, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0xd, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9 };
Texture Asset_PillYellow_A = {
        &Texture_VTABLE_DEFAULT,
        7, 7,
        PillYellow_A_TextureData
};

/* *************************************************************** */
/* Right Sided                                                      */
/* *************************************************************** */

static unsigned char PillBlue_B_TextureData[] = { 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0xd, 0x9, 0x9, 0x9, 0x9, 0x9, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0xd };
Texture Asset_PillBlue_B = {
        &Texture_VTABLE_DEFAULT,
        7, 7,
        PillBlue_B_TextureData
};

static unsigned char PillRed_B_TextureData[] = { 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0xd, 0x9, 0x9, 0x9, 0x9, 0x9, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0xd };
Texture Asset_PillRed_B = {
        &Texture_VTABLE_DEFAULT,
        7, 7,
        PillRed_B_TextureData
};

static unsigned char PillYellow_B_TextureData[] = { 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0xd, 0x6, 0x6, 0x6, 0x6, 0x6, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0xd };
Texture Asset_PillYellow_B = {
        &Texture_VTABLE_DEFAULT,
        7, 7,
        PillYellow_B_TextureData
};
