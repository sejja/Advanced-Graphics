//
//	Material.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 18/04/24
//	Copyright © 2024. All Rights reserved
//

#include "Material.h"

namespace Graphics {
    namespace Primitives {
        // ------------------------------------------------------------------------
        /*! Set Shininess
        *
        *  Sets the Shininess intensity of a Material
        */ // ---------------------------------------------------------------------
        void Material::SetShininess(const float n) {
            if (n < 0.f || n > 1.f)
                throw MaterialException("n is not in the range of 0 to 1");

            mShininess = n;
        }
    }
}