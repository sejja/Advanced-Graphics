//
//	PointLight.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 18/03/24
//	Copyright � 2024 . All Rights reserved
//

#include "PointLight.h"
#include "Graphics/Architecture/LightPass.h"

namespace Graphics {
	namespace Primitives {
		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*  Constructor for the PointLight class
		*/ //----------------------------------------------------------------------
		PointLight::PointLight(const std::weak_ptr<Core::Object>& parent)
			: Light(parent) {
				mData = new PointLightData();
				Graphics::Architecture::LightPass::AddPointLight(mIndex, std::shared_ptr<PointLightData>((PointLightData*)mData));
		}

		float PointLight::PointLightData::CalculateSphereOfInfluence() const {
			return 2 * mRadius;
		}
		void PointLight::PointLightData::GenerateShadowMap() {
		}
	}
}