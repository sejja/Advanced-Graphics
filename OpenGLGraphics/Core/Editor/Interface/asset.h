#ifndef ASSET_H
#define ASSET_H

#include <string>

class Asset {
	public:
		std::string tipo;
		std::string nombre;
		std::string ruta;

		Asset();
		Asset(std::string p_tipo, std::string p_nombre, std::string p_ruta);

		void dibujar();
};

#endif;
