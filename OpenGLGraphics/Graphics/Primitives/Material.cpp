//
//	Material.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 09/08/23
//	Copyright © 2023. All Rights reserved
//

#include "Material.h"
#include <exception>

namespace Graphics {
    // ------------------------------------------------------------------------
    /*! Set Shininess
    *
    *  Sets the Shininess intensity of a Material
    */ // ---------------------------------------------------------------------
	void Material::SetShininess(const float n) {
        if (n < 0.f || n > 1.f)
            throw std::exception("n is not in the range of 0 to 1");
            
		mShininess = n;
	}
}