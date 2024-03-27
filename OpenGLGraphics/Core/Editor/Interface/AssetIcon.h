#ifndef __ASSET_ICON_H__
#define __ASSET_ICON_H__

#include <string>

enum class AssetType {
	MODEL,
	TEXTURE,
	MATERIAL,
	SHADER,
	FOLDER,
	OTHER
};

class AssetIcon {
	public:
		AssetType tipo;
		const char* nombre;
		const char* ruta;
		bool clicked;

		AssetIcon();

		AssetIcon(AssetType p_tipo, const char* p_nombre, const char* p_ruta);

		void dibujar(bool dibujarToolTip);
};

#endif;
