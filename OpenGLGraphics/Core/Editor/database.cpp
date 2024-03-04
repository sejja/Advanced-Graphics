#include "database.h"
#include <iostream>
#include "Dependencies/SQLite3/sqlite3.h"
#include "Interface/AssetIcon.h"
#include <vector>
#include "string.h"

Database::Database(std::string nombre) {
	//connection = NULL;
	sqlite3_open(nombre.c_str(), &connection);
}

Database::~Database() {
	sqlite3_close(connection);
}

std::vector<AssetIcon> Database::getFilesOfFolder(std::string nombre) {
	std::vector<AssetIcon> assets;
	std::string sql = "SELECT * FROM CARPETA WHERE ID_CARPETA_SUPERIOR IN (SELECT ID FROM CARPETA WHERE RUTA ='" + nombre + "');";
	sqlite3_stmt* stmt;
	int i = sqlite3_prepare_v2(connection, sql.c_str(), -1, &stmt, NULL);
	std::cout << sqlite3_errstr(i) << std::endl;
	while (sqlite3_step(stmt) == SQLITE_ROW) {

		const char* nombreTemporal = (const char*)sqlite3_column_text(stmt, 1);
		const char* rutaTemporal = (const char*)sqlite3_column_text(stmt, 2);
		//char* nombre = malloc(strlen(nombreTemporal) + 1);
		char* nombre = new char[strlen(nombreTemporal) + 1];
		strcpy(nombre, nombreTemporal);
		char* ruta = new char[strlen(rutaTemporal) + 1];
		//char* ruta = malloc(strlen(rutaTemporal) + 1);
		strcpy(ruta, rutaTemporal);

		//std::string tipo = (const char*) sqlite3_column_text(stmt, 3);
		assets.push_back(AssetIcon(AssetType::FOLDER, nombre, ruta));
	}
	sqlite3_finalize(stmt);

	sql = "SELECT * FROM ASSET WHERE CARPETA IN (SELECT ID FROM CARPETA WHERE RUTA ='"+ nombre + "');";
	sqlite3_stmt* stmt2;
	i = sqlite3_prepare_v2(connection, sql.c_str(), -1, &stmt2, NULL);
	std::cout << sqlite3_errstr(i) << std::endl;
	while (sqlite3_step(stmt2) == SQLITE_ROW) {
		
		const char* nombreTemporal = (const char*) sqlite3_column_text(stmt2, 1);
		const char* rutaTemporal = (const char*) sqlite3_column_text(stmt2, 2);
		//char* nombre = malloc(strlen(nombreTemporal) + 1);
		char* nombre = new char[strlen(nombreTemporal) + 1];
		strcpy(nombre, nombreTemporal);
		char* ruta = new char[strlen(rutaTemporal) + 1];
		//char* ruta = malloc(strlen(rutaTemporal) + 1);
		strcpy(ruta, rutaTemporal);

		//std::string tipo = (const char*) sqlite3_column_text(stmt, 3);
		assets.push_back(AssetIcon(AssetType::OTHER, nombre, ruta));
	}
	sqlite3_finalize(stmt2);
	return assets;
}

void Database::closeConnection() {
	sqlite3_close(connection);
}