#include "windows.h"
#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include "asset.h"
#include "sqlite3.h"
#include "folder.h"

void cargarDirectorio(char incomePath[], sqlite3* database, int folderCode);
void emptyDatabase(sqlite3* database);

int main(int argc, char* argv[]) {

    char* actualPath;

    /*_getcwd(actualPath, sizeof(actualPath));

    printf("El directorio actual es: %s\n", actualPath);

    strcat(actualPath, "\\folderI");*/

    actualPath = argv[1];

    printf("%s\n", actualPath);

    //Conexión con base de datos
    sqlite3* database;
    
    

    int i = sqlite3_open("database.db", &database);
    printf("Database pointer: %p\n", database);
    printf("Codigo Apertura: %s\n", sqlite3_errstr(i)); 

    emptyDatabase(database);

    //sqlite3_stmt* ppStmt;
    // i = sqlite3_prepare_v2(database, "INSERT INTO ASSET('NOMBRE', 'RUTA') VALUES ('AAAA','AAAA')", -1, &ppStmt, NULL);
    // printf("Codigo preparacion: %s\n", sqlite3_errstr(i)); 
    // sqlite3_step(ppStmt);
    // printf("Codigo ejecucion: %s\n", sqlite3_errstr(i)); 
    // sqlite3_finalize(ppStmt);
    // printf("Codigo cierre query: %s\n", sqlite3_errstr(i));

    
    Folder* folder = createFolder(actualPath, 0); //Esto igual no tiene sentido
    //saveFolder(folder, database);
    /*char* sqlString[300];
    sprintf(sqlString, "INSERT INTO CARPETA(ID, NOMBRE, RUTA) VALUES(%d, '%s', '%s');", folder->code, folder->name, folder->path);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(database, sqlString, -1, &stmt, NULL);
    i = sqlite3_step(stmt);
    printf("Excuted. Code: %d\n", i);
    printf("Error String: %s\n", sqlite3_errstr(i));
    sqlite3_finalize(stmt);
    printf("Asset saved\n");
    destroyFolder(folder);*/
    
    cargarDirectorio(actualPath, database, 0);

    

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

int folderCode;

void cargarDirectorio(char incomePath[], sqlite3* database, int parentFolderCode) {
    printf("Folder code: %d\n", parentFolderCode);
    //printf("Database pointer: %p\n", database);
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

        char newPath[200];
        strcpy(newPath, incomePath);

        strcat(newPath, "\\");
        char* charString = toCharString(data.cFileName);
        strcat(newPath, charString);
        free(charString);


        if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )  //Comprobar si es un directorio
        {
            wprintf(L"Es un directorio: {\n");
            printf("Path: %s\n", newPath);

            Folder* folder = createFolder(newPath, ++folderCode);
            saveFolder(folder, database, parentFolderCode);
            destroyFolder(folder);

            cargarDirectorio(newPath, database, folderCode);

            printf("}\n");
        } else {

            //printf("Folder code %d", folderCode);
            Asset* asset = createAsset(newPath);
            /*if (asset->type == OTHER) {
                printf("El tipo es el correcto\n");
            }*/
            saveAsset(asset, database, parentFolderCode);
            destroyAsset(asset);
            free(asset);
            asset = NULL;
        }
    }

    FindClose(handle);


}

void emptyDatabase(sqlite3* database) {
    char* sqlString = "DELETE FROM ASSET;";
    //sqlite3_stmt* stmt;
    int i = sqlite3_exec(database, sqlString, NULL, NULL, NULL);
    printf("Error code %s", sqlite3_errstr(i));
    char* sqlString2 = "DELETE FROM CARPETA;";
    //sqlite3_stmt* stmt2;
    i = sqlite3_exec(database, sqlString2, NULL, NULL, NULL);
    printf("Error code %s", sqlite3_errstr(i));
}