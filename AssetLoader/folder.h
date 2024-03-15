#ifndef FOLDER_H
#define FOLDER_H

#include "sqlite3.h"

typedef struct {
	char *name;
	char *path;
	int code;
} Folder;

Folder* createFolder(char* path, int code);
void destroyFolder(Folder* folder);
int saveFolder(Folder* folder, sqlite3* database, int parentFolderCode);

#endif
