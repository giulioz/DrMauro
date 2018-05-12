/*
 *  Texture.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef TEXTURE_H
#define TEXTURE_H

#include "AssetManager.h"

typedef enum {
    TextureData_Unknown,
    TextureData_Grayscale,
    TextureData_Paletted_8,
    TextureData_RGB
} TextureDataType;

typedef struct {
    _BaseAsset_Implementation_
    size_t Width, Height;
    void* TextureData;
    TextureDataType DataType;
} Texture;

void Texture_Unload(void* texture);
Texture* Texture_Create(const char* name, TextureDataType dataType, size_t width, size_t height);
Texture* Texture_LoadFromFileRGB(const char* filename, AssetManager* assetManager);

#endif
