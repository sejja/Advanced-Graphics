//
//	DirectionalLight.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 18/03/24
//	Copyright � 2024 . All Rights reserved
//

#ifndef _DIRECTIONAL_LIGHT_H_
#define _DIRECTIONAL_LIGHT_H_

#include "Light.h"
#include "Shadows/CascadedShadowMap.h"
#include "Graphics/Primitives/ShaderProgram.h"

namespace Graphics {
	namespace Primitives {
		namespace Lights {
			class DirectionalLight : public Light {
#pragma region //Declarations
			public:
				struct DirectionalLightData : public Light::BackedLightData {
#pragma region //Methods
					DONTDISCARD float CalculateSphereOfInfluence() const noexcept override;
					void inline RenderShadowsMap(const glm::mat4& camview, const std::function<void(Core::Graphics::ShaderProgram*)>& rend_func);
					void inline SetUniforms(const Core::Assets::Asset<Core::Graphics::ShaderProgram>& shader);
#pragma endregion

#pragma region //Members
					glm::vec3 mDirection;
					Lights::Shadows::CascadedShadowMap mShadowMap;
#pragma endregion
				};
#pragma endregion

#pragma region //Constructor & Destructor
				DirectionalLight(const std::weak_ptr<Core::Object>& parent);
				~DirectionalLight();
#pragma endregion

#pragma region //Methods
				DONTDISCARD glm::vec3 inline GetDirection() const noexcept;
				void inline SetDirection(const glm::vec3& direction) noexcept;
			};

			// ------------------------------------------------------------------------
			/*! Get Direction
			*
			*  Gets the Direction the light impacts the surfaces
			*/ //----------------------------------------------------------------------
			glm::vec3 DirectionalLight::GetDirection() const noexcept {
				return std::reinterpret_pointer_cast<DirectionalLightData>(mData)->mDirection;
			}

			// ------------------------------------------------------------------------
			/*! Set Direction
			*
			*  Sets the Direction in which the light impacts the surfaces
			*/ //----------------------------------------------------------------------
			void DirectionalLight::SetDirection(const glm::vec3& direction) noexcept {
				 std::reinterpret_pointer_cast<DirectionalLightData>(mData)->mDirection = direction;
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

#endif