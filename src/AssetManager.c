/*
 *  AssetManager.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  Manage asset loading and freeing
 */

#include "AssetManager.h"
#include "ExceptionManager.h"

#include <string.h>

AssetManager AssetManager_Create(const char* baseFolder) {
    AssetManager am;
    am.Assets = Vector_Create(AssetManager_InitialSize, sizeof(void*));
    am.BaseFolder = baseFolder;
    return am;
}

void AssetManager_Add(AssetManager* assetManager, BaseAsset* asset) {
    Vector_Add(&assetManager->Assets, &asset);
}

BaseAsset* AssetManager_Get(AssetManager* assetManager, const char* name) {
    size_t i;
    Vector_Foreach(assetManager->Assets, i) {
        BaseAsset* asset = *(BaseAsset**)Vector_Get(&assetManager->Assets, i);
        if (strcmp(asset->AssetName, name) == 0) {
            return asset;
        }
    }
    ThrowError(ErrorConcat("Cannot find asset:", name));
    return NULL;
}

void AssetManager_Unload(AssetManager* assetManager, const char* name) {
    size_t i;
    Vector_Foreach(assetManager->Assets, i) {
        BaseAsset* asset = *(BaseAsset**)Vector_Get(&assetManager->Assets, i);
        if (strcmp(asset->AssetName, name) == 0) {
            asset->Asset_Unload(asset);
            Vector_Delete(&assetManager->Assets, i);
            return;
        }
    }
    ThrowError(ErrorConcat("Cannot find asset:", name));
}

void AssetManager_UnloadAll(AssetManager* assetManager) {
    size_t i;
    Vector_Foreach(assetManager->Assets, i) {
        BaseAsset* asset = *(BaseAsset**)Vector_Get(&assetManager->Assets, i);
        asset->Asset_Unload(asset);
    }
    Vector_Clear(&assetManager->Assets);
}

char* AssetManager_GetFullPath(AssetManager* assetManager, const char* filename) {
    char* finalString = calloc(strlen(assetManager->BaseFolder) + strlen(filename) + 1, 1);
    strcpy(finalString, assetManager->BaseFolder);
    strcat(finalString, filename);
    return finalString;
}
