#include "sqlite3.h"
#include <stdio.h>

int main1() {
    sqlite3* database;
    sqlite3_open("database.db", &database);
    printf("1\n");

    sqlite3_stmt* ppStmt;
    sqlite3_prepare_v2(database, "SELECT * FROM ASSET", -1, &ppStmt, NULL);
    // printf("2\n");
    // int i = sqlite3_exec(database, "SELECT * FROM ASSET", NULL, NULL, NULL);
    // int i = sqlite3_step(ppStmt);
    while (sqlite3_step(ppStmt) == SQLITE_ROW) {
        const char* valor = sqlite3_column_text(ppStmt, 1);
        printf("%s\n", valor);
    }
    // printf("Error String: %s\n", sqlite3_errstr(i));
    printf("3\n");

    sqlite3_finalize(ppStmt);
    int i = sqlite3_close(database);
    printf("Codigo cierre: %s\n", sqlite3_errstr(i));
    printf("4\n");
    return 0;
}