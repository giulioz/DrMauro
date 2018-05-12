/*
 *  AssetManager.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  Manage asset loading and freeing
 */

#include "AssetManager.h"
#include "ExceptionManager.h"

#include <string.h>
#include <AssetManager.h>

AssetManager AssetManager_Create(const char* baseFolder) {
    AssetManager am;
    am.Assets = Vector_Create(AssetManager_InitialSize, sizeof(void*));
    am.BaseFolder = baseFolder;
    return am;
}

void AssetManager_Add(AssetManager* assetManager, BaseAsset* asset) {
    Vector_Add(&assetManager->Assets, asset);
}

BaseAsset* AssetManager_Get(AssetManager* assetManager, const char* name) {
    int i;
    Vector_Foreach(assetManager->Assets, i) {
        BaseAsset* asset = (BaseAsset*)Vector_Get(&assetManager->Assets, i);
        if (strcmp(asset->AssetName, name) == 0) {
            return asset;
        }
    }
    ThrowError(ErrorConcat("Cannot find asset:", name));
    return NULL;
}

void AssetManager_Unload(AssetManager* assetManager, const char* name) {
    BaseAsset* asset = AssetManager_Get(assetManager, name);
    asset->Asset_Unload(asset);
}

void AssetManager_UnloadAll(AssetManager* assetManager) {
    int i;
    Vector_Foreach(assetManager->Assets, i) {
        ((BaseAsset*)Vector_Get(&assetManager->Assets, i))
                ->Asset_Unload(Vector_Get(&assetManager->Assets, i));
    }
    Vector_Clear(&assetManager->Assets);
}

char* AssetManager_GetFullPath(AssetManager* assetManager, const char* filename) {
    char* finalString = calloc(strlen(assetManager->BaseFolder) + strlen(filename) + 1, 1);
    strcpy(finalString, assetManager->BaseFolder);
    strcat(finalString, filename);
    return finalString;
}
