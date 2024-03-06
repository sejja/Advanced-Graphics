#ifndef __ASSET_MANAGER__H__
#define __ASSET_MANAGER__H__

#include <vector>
#include "AssetIcon.h"
#include "../database.h"

class AssetManager {
public:
	~AssetManager();
	AssetManager();
	void Render();

private:
	int elementosPorFila(int anchoVentana, int anchoElemento);
	std::vector<AssetIcon> assets;
	Core::Editor::Database* db;
};

#endif
