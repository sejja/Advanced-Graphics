#ifndef __ASSET_ICON_H__
#define __ASSET_ICON_H__

#include <string>

class AssetIcon {
	public:
		std::string tipo;
		std::string nombre;
		std::string ruta;

		AssetIcon();
		AssetIcon(std::string p_tipo, std::string p_nombre, std::string p_ruta);

		void dibujar();
};

#endif
