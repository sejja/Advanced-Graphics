#include "database.h"
#include <iostream>
#include "Dependencies/SQLite3/sqlite3.h"
#include "Interface/AssetIcon.h"
#include <vector>
#include "string.h"

namespace Core {
	namespace Editor {
		Database::Database(std::string nombre) {
			//connection = NULL;
			sqlite3_open(nombre.c_str(), &connection);
		}

		Database::~Database() {
			closeConnection();
		}

		std::vector<AssetIcon> Database::getFilesOfFolder(std::string nombre) {
			std::vector<AssetIcon> assets;
			std::string sql = "SELECT * FROM CARPETA WHERE ID_CARPETA_SUPERIOR IN (SELECT ID FROM CARPETA WHERE RUTA ='" + nombre + "');";
			//printf("%s\n", sql.c_str());
			appendFilesOfStatement(sql, assets);

			sql = "SELECT * FROM ASSET WHERE CARPETA IN (SELECT ID FROM CARPETA WHERE RUTA ='" + nombre + "');";
			//printf("%s\n", sql.c_str());
			appendFilesOfStatement(sql, assets);
			return std::move(assets);
		}

		std::vector<AssetIcon> Database::getFilesOfRoot() {
			std::vector<AssetIcon> assets;
			std::string sql = "SELECT * FROM CARPETA WHERE ID_CARPETA_SUPERIOR IS NULL;";
			appendFilesOfStatement(sql, assets);

			sql = "SELECT * FROM ASSET WHERE CARPETA IS NULL;";
			appendFilesOfStatement(sql, assets);
			return std::move(assets);
		}

		void Database::closeConnection() {
			printf("Cerrando base de datos\n");
			sqlite3_close(connection);
		}

		void Database::appendFilesOfStatement(std::string sql, std::vector<AssetIcon>& assets) {
			sqlite3_stmt* stmt;
			printf("%s\n", sql.c_str());
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
		}
	}
}