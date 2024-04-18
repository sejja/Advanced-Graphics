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
			/*! Calculate Sphere Of Influence
			*
			*  Calculates the sphere of influence of the light
			*/ //----------------------------------------------------------------------
			float DirectionalLight::DirectionalLightData::CalculateSphereOfInfluence() const {
				return std::numeric_limits<float>::max();
			}

			// ------------------------------------------------------------------------
			/*! Render Shadow Map
			*
			*  REnders the shadow map depth buffer
			*/ //----------------------------------------------------------------------
			void DirectionalLight::DirectionalLightData::RenderShadowsMap(const glm::mat4& camview, const std::function<void(Core::Graphics::ShaderProgram*)>& rend_func) {
				mShadowMap.Render(camview, mPosition, mDirection, rend_func);
			}

			// ------------------------------------------------------------------------
			/*! Set Uniforms
			*
			*  Sets the Uniforms to the shaders needed to render the light's shadow
			*/ //----------------------------------------------------------------------
			void DirectionalLight::DirectionalLightData::SetUniforms(const Core::Assets::Asset<Core::Graphics::ShaderProgram>& shader) {
				mShadowMap.SetUniforms(shader);
			}
		}
	}
}