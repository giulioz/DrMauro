/*
 *  AssetManager.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  Manage asset loading and freeing
 */

#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include "Vector.h"
#include "Object.h"

typedef class Asset {
    void (*unload)(class Asset* asset);
    char* AssetName;
} Asset;

typedef struct {
    const char* BaseFolder;
    Vector Assets;
} AssetManager;

AssetManager AssetManager_Create(const char* baseFolder); /* baseFolder must be static!! */
BaseAsset* AssetManager_Get(AssetManager* assetManager, const char* name);
void AssetManager_Add(AssetManager* assetManager, BaseAsset* asset);
void AssetManager_Unload(AssetManager* assetManager, const char* name);
void AssetManager_UnloadAll(AssetManager* assetManager);
char* AssetManager_GetFullPath(AssetManager* assetManager, const char* filename);

#endif
