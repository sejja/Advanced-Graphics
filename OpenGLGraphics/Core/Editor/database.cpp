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
			appendFilesOfStatement(sql, assets, true);

			sql = "SELECT * FROM ASSET WHERE CARPETA IN (SELECT ID FROM CARPETA WHERE RUTA ='" + nombre + "');";
			//printf("%s\n", sql.c_str());
			appendFilesOfStatement(sql, assets, false);
			return std::move(assets);
		}

		std::vector<AssetIcon> Database::getFilesOfRoot() {
			std::vector<AssetIcon> assets;
			std::string sql = "SELECT * FROM CARPETA WHERE ID_CARPETA_SUPERIOR IS NULL;";
			appendFilesOfStatement(sql, assets, true);

			sql = "SELECT * FROM ASSET WHERE CARPETA IS NULL;";
			appendFilesOfStatement(sql, assets, false);
			return std::move(assets);
		}

		void Database::closeConnection() {
			printf("Cerrando base de datos\n");
			sqlite3_close(connection);
		}

		void Database::appendFilesOfStatement(std::string sql, std::vector<AssetIcon>& assets, bool folder) {
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
				if (folder) {
					assets.push_back(AssetIcon(AssetType::FOLDER, nombre, ruta));
				}
				else {
					const char* tipoCharArray = (const char*)sqlite3_column_text(stmt, 3);
					//char* tipoCharArray = new char[strlen(tipoTemporal) + 1];
					//strcpy(tipoCharArray, tipoTemporal);
					AssetType tipo = getAssetType(tipoCharArray);
					assets.push_back(AssetIcon(tipo, nombre, ruta));
				}
				
			}
			sqlite3_finalize(stmt);
		}

		AssetType Database::getAssetType(const char* tipo) {
			if (strcmp(tipo, "MODEL") == 0) {
				return AssetType::MODEL;
			}
			else if (strcmp(tipo, "TEXTURE") == 0) {
				return AssetType::TEXTURE;
			}
			else if (strcmp(tipo, "MATERIAL") == 0) {
				return AssetType::MATERIAL;
			}
			else if (strcmp(tipo, "SHADER") == 0) {
				return AssetType::SHADER;
			}
			else {
				return AssetType::OTHER;
			}
		}
	}
}