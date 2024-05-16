#ifndef __ASSET_ICON_H__
#define __ASSET_ICON_H__

#include <string>
#include <unordered_map>

enum class AssetType {
	MODEL,
	TEXTURE,
	MATERIAL,
	SHADER,
	FOLDER,
	LEVEL,
	OTHER
};

class AssetIcon {
	public:
		static std::unordered_map<AssetType, std::string> assetTypeImages;
		AssetType tipo;
		const char* nombre;
		const char* ruta;
		bool clicked;

		AssetIcon();

		AssetIcon(AssetType p_tipo, const char* p_nombre, const char* p_ruta);

		void dibujar(bool dibujarToolTip);
};

//std::unordered_map<AssetType, std::string> AssetIcon::assetTypeImages = std::unordered_map<AssetType, std::string>();

#endif;
