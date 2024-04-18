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
		class DirectionalLight : public Light {
		#pragma region //Declarations
		public:
			struct DirectionalLightData : public Light::BackedLightData {
			#pragma region //Methods
				DONTDISCARD float CalculateSphereOfInfluence() const override;
				void RenderShadowsMap(const glm::mat4& camview, const std::function<void(Core::Graphics::ShaderProgram*)>& rend_func);
				void SetUniforms(const Core::Assets::Asset<Core::Graphics::ShaderProgram>& shader);
			#pragma endregion

			#pragma region //Members
				glm::vec3 mDirection;
				Lights::Shadows::CascadedShadowMap mShadowMap;
			#pragma endregion
			};
		#pragma endregion

		#pragma region //Constructor & Destructor
			DirectionalLight(const std::weak_ptr<Core::Object>& parent);
		#pragma endregion
		};
	}
}

#endif