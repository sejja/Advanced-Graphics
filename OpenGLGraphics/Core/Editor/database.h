#ifndef __DATABASE__H__
#define __DATABASE__H__

#include "Dependencies/SQLite3/sqlite3.h"
#include "Interface/AssetIcon.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include "Interface/AssetIcon.h"

namespace Core {
	namespace Editor {
		class Database {
		public:
			Database(std::string nombre);
			~Database();
			void closeConnection();
			std::vector<AssetIcon> getFilesOfFolder(std::string nombre);
			std::vector<AssetIcon> getFilesOfRoot();
			std::unordered_map<AssetType, std::string> getAssetTypeImages();

		private:
			sqlite3* connection;
			void appendFilesOfStatement(std::string sql, std::vector<AssetIcon>& assets, bool folder);
			AssetType getAssetType(const char* tipo);

		};
	}
}
#endif

