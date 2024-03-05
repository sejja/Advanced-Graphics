#ifndef __DATABASE__H__
#define __DATABASE__H__

#include "Dependencies/SQLite3/sqlite3.h"
#include "Interface/AssetIcon.h"
#include <iostream>
#include <vector>

class Database {
public:
	Database(std::string nombre);
	~Database();
	void closeConnection();
	std::vector<AssetIcon> getFilesOfFolder(std::string nombre);

private:
	sqlite3* connection;
};

#endif

