#ifndef __ASSET_ICON_H__
#define __ASSET_ICON_H__

#include <string>

enum class AssetType {
	MODEL,
	TEXTURE,
	MATERIAL,
	SHADER,
	OTHER
};

class AssetIcon {
	public:
		AssetType tipo;
		const char* nombre;
		const char* ruta;

		AssetIcon();
		AssetIcon(AssetType p_tipo, const char* p_nombre, const char* p_ruta);

		void dibujar();
};

#endif;
