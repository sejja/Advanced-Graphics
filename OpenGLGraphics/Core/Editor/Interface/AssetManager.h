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
	void createFile(const char* filename);
	std::vector<AssetIcon> assets;
	AssetIcon folder; //Esto debería ser un puntero? ns bro
	std::stack<AssetIcon> previousFolders;

	char filename[256];
	bool showFileDialog = false;
};

#endif
