//
//	Light.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 18/04/24
//	Copyright � 2024 . All Rights reserved
//

#include "Light.h"

namespace Graphics {
	namespace Primitives {
		namespace Lights {
			// ------------------------------------------------------------------------
			/*! Conversion Constructor
			*
			*   Constructs a light with it's parent as a reference
			*/ //----------------------------------------------------------------------
			Light::Light(const std::weak_ptr<Core::Object>& parent) :
				Component(parent) {
			}

			// ------------------------------------------------------------------------
			/*! Set Position
			*
			*   Sets the position of the light, relative to the parent
			*/ //----------------------------------------------------------------------
			void Light::SetPosition(const glm::vec3& relativePos) noexcept {
				//If there is no parent, return
				if(GetParent().expired()) return;

				mData->mPosition = GetParent().lock()->GetPosition() + relativePos;
			}
			
			// ------------------------------------------------------------------------
			/*! Constructor
			*
			*   Default initialices the Light data
			*/ //----------------------------------------------------------------------
			Light::BackedLightData::BackedLightData() :
				mColor(), mPosition(), mShadowCaster() {
			}
		}
	}
}