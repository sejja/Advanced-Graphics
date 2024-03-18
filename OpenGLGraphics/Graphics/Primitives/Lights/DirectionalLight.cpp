//
//	DirectionalLight.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 18/03/24
//	Copyright � 2024 . All Rights reserved
//

#include "DirectionalLight.h"
#include "Graphics/Architecture/LightPass.h"

namespace Graphics {
	namespace Primitives {
		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*  Constructor for the DirectionalLight class
		*/ //----------------------------------------------------------------------
		DirectionalLight::DirectionalLight(const std::weak_ptr<Core::Object>& parent) 
			: Light(parent) {
			mData = new DirectionalLightData;
			Graphics::Architecture::LightPass::sDirectionalLightData.insert(std::make_pair(mIndex, (DirectionalLightData*)mData));
		}

		float DirectionalLight::DirectionalLightData::CalculateSphereOfInfluence() const {
			return -1.0f;
		}
		void DirectionalLight::DirectionalLightData::GenerateShadowMap() {
		}
	}
}