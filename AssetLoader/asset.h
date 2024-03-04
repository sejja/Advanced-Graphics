#include "sqlite3.h"

#ifndef ASSET_H
#define ASSET_H

enum AssetType
{
    MODEL,
    TEXTURE,
    MATERIAL,
    SHADER,
    OTHER
};

typedef struct
{
    char *name;
    char *path;
    enum AssetType type;
} Asset;

Asset* createAsset(char* path);
void destroyAsset(Asset* asset);
int saveAsset(Asset* asset, sqlite3* database);

#endif
