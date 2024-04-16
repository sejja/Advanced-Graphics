//
//	SpotLight.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 18/03/24
//	Copyright � 2024 . All Rights reserved
//

#include "SpotLight.h"
#include "Graphics/Architecture/LightPass.h"

namespace Graphics {
	namespace Primitives {
		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*  Constructor for the SpotLight class
		*/ //----------------------------------------------------------------------
		SpotLight::SpotLight(const std::weak_ptr<Core::Object>& parent)
			: Light(parent) {
				mData = new SpotLightData();
				Graphics::Architecture::LightPass::AddSpotLight(mIndex, std::shared_ptr<SpotLightData>((SpotLightData*)mData));
				((SpotLightData*)mData)->GenerateShadowMap();
		}

		float SpotLight::SpotLightData::CalculateSphereOfInfluence() const {
			return 2 * mRadius;
		}
		void SpotLight::SpotLightData::GenerateShadowMap() {
			mShadowMap.Create();
			mShadowMap.CreateRenderTexture({1600, 900});
		}
	}
}