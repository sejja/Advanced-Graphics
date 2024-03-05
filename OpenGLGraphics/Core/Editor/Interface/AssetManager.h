#ifndef __ASSET_MANAGER__H__
#define __ASSET_MANAGER__H__

#include <vector>
#include "AssetIcon.h"

class AssetIcon;

class AssetManager {
public:
	AssetManager();
	void Render();

private:
	int elementosPorFila(int anchoVentana, int anchoElemento);
	std::vector<AssetIcon> assets;
};

#endif
