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
		namespace Lights {
			// ------------------------------------------------------------------------
			/*! Default Constructor
			*
			*  Constructor for the DirectionalLight class
			*/ //----------------------------------------------------------------------
			DirectionalLight::DirectionalLight(const std::weak_ptr<Core::Object>& parent)
				: Light(parent) {
				mData = std::make_shared<DirectionalLightData>();
				Graphics::Architecture::LightPass::AddDirectionalLight(std::reinterpret_pointer_cast<DirectionalLightData>(mData));
			}

			// ------------------------------------------------------------------------
			/*! Destructor
			*
			*  Removes the Directional Light from the Light Pass
			*/ //----------------------------------------------------------------------
			DirectionalLight::~DirectionalLight() {
				Graphics::Architecture::LightPass::RemoveDirectionalLight(std::reinterpret_pointer_cast<DirectionalLightData>(mData));
			}

			// ------------------------------------------------------------------------
			/*! Calculate Sphere Of Influence
			*
			*  Calculates the sphere of influence of the light
			*/ //----------------------------------------------------------------------
			float DirectionalLight::DirectionalLightData::CalculateSphereOfInfluence() const noexcept {
				return std::numeric_limits<float>::max();
			}
		}
	}
}