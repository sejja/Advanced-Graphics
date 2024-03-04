#include "asset.h"
#include "stdio.h"
#include "stdlib.h"
// #include "stdbool.h"
#include "string.h"
#include "sqlite3.h"

void destroyAsset(Asset* asset) {
    // printf("Entrando en destroy asset");
    free(asset->name);
    // printf("Nombre liberado");
    // free(asset->path); Hum
    // printf("Asset Destruido");
}

Asset* createAsset(char* path) {
    // printf(path);
    // printf("Creando Asset");
    char aTonkenizar[100];
    strcpy(aTonkenizar, path);
    char* token = strtok(aTonkenizar, "\\");
    // printf(aTonkenizar);
    char* previousValue;
    while(token != NULL) {
        // printf("%s\n", token);
        previousValue = token;
        token = strtok(NULL, "\\");
    }
    char nombreExtension[50];
    // printf("Previous value: %s\n", previousValue);
    strcpy(nombreExtension, previousValue);
    char* nombre = malloc(50 * sizeof(char)); //Cuando puedo liberar esto?
    strcpy(nombre, strtok(nombreExtension, ".")); //Puede que el nombre no se pase porque se guarda en stack y se libera al salir de la función?
    // printf("Nombre: %s\n", nombre);
    char* extension = strtok(NULL, ".");
    // printf("Extension: %s\n", extension);
    
    Asset* asset = malloc(sizeof(Asset));
    asset->name = nombre;
    asset->path = path;
    
    // printf("Nombre: %s\n", asset->name);
    // asset->type = OTHER;
    // return asset;

    if (strcmp(extension, "obj") == 0) {
        asset->type = MODEL;
    }
    else if (strcmp(extension, "png") == 0) {
        asset->type = TEXTURE;
    }
    else if (strcmp(extension, "mtl") == 0) {
        asset->type = MATERIAL;
    }
    else if (strcmp(extension, "shader") == 0) {
        asset->type = SHADER;
    }
    else {
        asset->type = OTHER;
    }

    return asset;
}

int saveAsset(Asset* asset, sqlite3* database) {
    printf("Saving asset\n");
    sqlite3_stmt* stmt;
    char stmtString[300];
    sprintf(stmtString, "INSERT INTO ASSET(NONBRE, RUTA) VALUES('%s','%s');", "bbbb", "bbbb"); //FALTA TIPO y carpeta
    const char* hola = "Hola";
    printf("SQL statement: %s\n", stmtString);
    sqlite3_prepare_v2(database, "INSERT INTO ASSET(NONBRE, RUTA) VALUES('1111','11111')", -1, &stmt, NULL);
    // printf(i);
    // printf("Statement prepared\n");
    int i = sqlite3_step(stmt);
    printf("Excuted. Code: %d\n", i);
    printf("Error String: %s\n", sqlite3_errstr(i));
    sqlite3_finalize(stmt);
    printf("Asset saved\n");
    // sqlite3_exec(database, stmtString, 0,0,0);

    // Commit the changes
    // const char* commitStatement = "COMMIT;";
    // sqlite3_exec(database, commitStatement, 0, 0, 0);
    // printf("Changes committed\n");
}