#include "folder.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

Folder* createFolder(char* path, int code) {
    char aTonkenizar[100];
    strcpy(aTonkenizar, path);
    //printf("A tokenizar: %s", aTonkenizar);
    char* token = strtok(aTonkenizar, "\\");
    char* previousValue = NULL;
    while (token != NULL) {
        //printf("%s\n", token);
        previousValue = token;
        token = strtok(NULL, "\\");
    }
    //char nombreExtension[50];
    // printf("Previous value: %s\n", previousValue);
    //strcpy(nombreExtension, previousValue);
    //char* nombre = malloc(50 * sizeof(char)); //Cuando puedo liberar esto?
    //char* nombre = "hola";
    //strcpy(nombre, strtok(nombreExtension, ".")); //Puede que el nombre no se pase porque se guarda en stack y se libera al salir de la función?
    // printf("Nombre: %s\n", nombre);
    //char* extension = strtok(NULL, ".");
    // printf("Extension: %s\n", extension);

    Folder* folder = (Folder*) malloc(sizeof(Folder));
    //printf("%p\n", folder);
    folder->code = code;
    folder->name = (char*)malloc(50 * sizeof(char));
    strcpy(folder->name, previousValue);
    folder->path = path;
    return folder;
}

void destroyFolder(Folder* folder) {
	//free(folder->name);
	// free(folder->path);
	// printf("Folder Destruido");
    free(folder);
    folder = NULL;
}

int saveFolder(Folder* folder, sqlite3* database, int parentFolderCode) {
    //printf("Saving folder\n");
    sqlite3_stmt* stmt;
    char stmtString[300];
    sprintf(stmtString, "INSERT INTO CARPETA(ID, NOMBRE, RUTA, ID_CARPETA_SUPERIOR) VALUES(%d, '%s', '%s', '%d');", folder->code, folder->name, folder->path, parentFolderCode);
    //const char* hola = "Hola";
    printf("SQL statement: %s\n", stmtString);
    sqlite3_prepare_v2(database, stmtString, -1, &stmt, NULL);
    // printf(i);
    // printf("Statement prepared\n");
    int i = sqlite3_step(stmt);
    //printf("Excuted. Code: %d\n", i);
    //printf("Error String: %s\n", sqlite3_errstr(i));
    sqlite3_finalize(stmt);
    printf("Asset saved\n");
    return 0;
}