/*
 *  Texture.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "Texture.h"

#include <stdlib.h>
#include <string.h>
#include <Font.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void Texture_Unload(void* texture) {
    free(((Texture*)texture)->AssetName);
    free(((Texture*)texture)->TextureData);
    if (((Texture*)texture)->TransparentKey) free(((Texture*)texture)->TransparentKey);
    free(texture);
}

Texture* Texture_Create(const char* name, TextureDataType dataType, size_t BPP, size_t width, size_t height) {
    Texture *texture = malloc(sizeof(Texture));

    texture->Asset_Unload = Texture_Unload;
    texture->AssetName = calloc(strlen(name) + 1, sizeof(char));
    strcpy(texture->AssetName, name);
    texture->DataType = dataType;
    texture->Width = width;
    texture->Height = height;
    texture->TransparentKeyed = false;
    texture->TransparentKey = NULL;
    texture->BPP = BPP;

    switch (dataType) {
        case TextureData_Grayscale:
        case TextureData_Paletted_8:
            texture->TextureData = calloc(width*height, 1);
            break;
        case TextureData_RGB:
            texture->TextureData = calloc(width*height, 3);
            break;
        default: /* unknown pixel format */
            texture->TextureData = NULL;
            break;
    }

    return texture;
}

Texture* Texture_CreateTransparent(const char* name, TextureDataType dataType, size_t BPP, size_t width, size_t height, void* key) {
    Texture *texture = Texture_Create(name, dataType, BPP, width, height);
    texture->TransparentKeyed = true;
    texture->TransparentKey = malloc(BPP);
    memcpy(texture->TransparentKey, key, BPP);
    return texture;
}

Texture* Texture_LoadFromFileRGB(const char* filename, AssetManager* assetManager) {
    int x,y,n;
    char* fullPath = AssetManager_GetFullPath(assetManager, filename);
    unsigned char *data = stbi_load(fullPath, &x, &y, &n, 3);
    Texture *texture = Texture_Create(filename, TextureData_Unknown, n, x, y);
    texture->TextureData = data;
    texture->DataType = TextureData_RGB;
    free(fullPath);
    return texture;
}

Texture* Texture_LoadFromFileRGB_Keyed(const char* filename, AssetManager* assetManager, void* key) {
    int x,y,n;
    char* fullPath = AssetManager_GetFullPath(assetManager, filename);
    unsigned char *data = stbi_load(fullPath, &x, &y, &n, 3);
    Texture *texture = Texture_CreateTransparent(filename, TextureData_Unknown, 3, x, y, key);
    texture->TextureData = data;
    texture->DataType = TextureData_RGB;
    free(fullPath);
    return texture;
}

bool Texture_TransparentKey(Texture* texture, const void* start) {
    if (!texture->TransparentKey) return true;
    else {
        return (bool)(memcmp(start, texture->TransparentKey, texture->BPP) != 0);
    }
}
