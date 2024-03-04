#include "windows.h"
#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include "asset.h"
#include "sqlite3.h"

void cargarDirectorio(char incomePath[], sqlite3* database);

int main() {

    char actualPath[100];

    _getcwd(actualPath, sizeof(actualPath));

    printf("El directorio actual es: %s\n", actualPath);

    strcat(actualPath, "\\folderI");

    //Conexión con base de datos
    sqlite3* database;
    int i = sqlite3_open("database.db", &database);
    printf("Codigo Apertura: %s\n", sqlite3_errstr(i)); 

    sqlite3_stmt* ppStmt;
    // i = sqlite3_prepare_v2(database, "INSERT INTO ASSET('NOMBRE', 'RUTA') VALUES ('AAAA','AAAA')", -1, &ppStmt, NULL);
    // printf("Codigo preparacion: %s\n", sqlite3_errstr(i)); 
    // sqlite3_step(ppStmt);
    // printf("Codigo ejecucion: %s\n", sqlite3_errstr(i)); 
    // sqlite3_finalize(ppStmt);
    // printf("Codigo cierre query: %s\n", sqlite3_errstr(i));

    cargarDirectorio(actualPath, database);

    // printf("Hola");
    // char assetStr[] = "ho\\joe.obj";
    // Asset* asset = createAsset(assetStr);
    // printf("%p", asset);
    // printf("%s - %s\n", asset->name, asset->path);
    // if (asset->type == OTHER) {
    //     printf("El tipo es el correcto");
    // }
    // printf("No se ha cerrado la base de datos");
    sqlite3_close(database);
    printf("Codigo cierre base de datos: %s\n", sqlite3_errstr(i));  
    printf("Se acabo");
    return 0;
}

char* toCharString(wchar_t* wString) {
    size_t wideStringLength = wcslen(wString);
    char* charString = (char*)malloc(wideStringLength + 1 * sizeof(char));
    wcstombs(charString, wString, wideStringLength + 1);
    return charString;
}

void cargarDirectorio(char incomePath[], sqlite3* database) {
    const char* path[100];
    WIN32_FIND_DATAW data;

    sprintf(path, "%s\\*", incomePath);

    wchar_t pathRaro[200]; //200 es una cantidad aceptable?

    //Esto no lo tengo muy claro
    setlocale(LC_ALL, "");  // set the locale
    mbstowcs(pathRaro, path, sizeof(pathRaro)/sizeof(wchar_t));  // convert to wide string

    HANDLE handle = FindFirstFileW(pathRaro, &data);


    if(handle == INVALID_HANDLE_VALUE) {
        wprintf(L"No se ha podido abrir el directorio\n");
        return;
    }

    FindNextFileW(handle, &data); //Esto es para que no muestre el . y el ..
    // FindNextFileW(handle, &data);
    while (FindNextFileW(handle, &data)) {
        wprintf(L"%s\n", data.cFileName);

        char newPath[100];
        strcpy(newPath, incomePath);

        strcat(newPath, "\\");
        char* charString = toCharString(data.cFileName);
        strcat(newPath, charString);
        free(charString);


        if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )  //Comprobar si es un directorio
        {
            wprintf(L"Es un directorio: {\n");
            printf("Path: %s\n", newPath);
            //Cast data.cFileName to char*
            // sprintf(newPath, "%s\\%s", incomePath, toCharString(data.cFileName));



            cargarDirectorio(newPath, database);

            printf("}\n");
        } else {

            Asset* asset = createAsset(newPath);
            // printf("%p\n", asset);
            // printf("%s - %s\n", asset->name, asset->path);
            if (asset->type == OTHER) {
                printf("El tipo es el correcto\n");
            }
            saveAsset(asset, database);
            // printf("Hola1");
            destroyAsset(asset);
            free(asset);
            asset = NULL;
            // printf("Hola");
        }
    }

    FindClose(handle);


}