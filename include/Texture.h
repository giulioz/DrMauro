/*
 *  Texture.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef TEXTURE_H
#define TEXTURE_H

typedef enum {
    TextureData_Monochrome,
    TextureData_Paletted_8,
    TextureData_RGB
} TextureDataType;

typedef struct {
    unsigned int Width, Height;
    void* TextureData;
    TextureDataType DataType;
} Texture;

#endif
