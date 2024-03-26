#ifndef __ASSET_MANAGER__H__
#define __ASSET_MANAGER__H__

#include <vector>
#include "AssetIcon.h"
#include "../database.h"
#include <stack>

class AssetManager {
public:
	AssetManager();
	void Render();
	//void changeDirectory(int i);
	void init();

private:
	int elementosPorFila(int anchoVentana, int anchoElemento);
	void gotoPreviousFolder();
	void changeDirectory(int i);
	std::vector<AssetIcon> assets;
	AssetIcon folder; //Esto debería ser un puntero?
	std::stack<AssetIcon> previousFolders;
};

#endif
