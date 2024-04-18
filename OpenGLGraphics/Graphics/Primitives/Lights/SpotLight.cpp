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
				mData = std::make_shared<SpotLightData>();
				Graphics::Architecture::LightPass::AddSpotLight(std::reinterpret_pointer_cast<SpotLightData>(mData));
				std::reinterpret_pointer_cast<SpotLightData>(mData)->GenerateShadowMap();
		}

		// ------------------------------------------------------------------------
		/*! Generate Shadow Map
		*
		*  Generates the Shadow Map for texturing the shadows
		*/ //----------------------------------------------------------------------
		void SpotLight::SpotLightData::GenerateShadowMap() {
			mShadowMap.Create();
			mShadowMap.CreateRenderTexture({1600, 900}, true, false);
		}
	}
}