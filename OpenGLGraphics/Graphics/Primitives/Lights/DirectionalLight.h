﻿//
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
		public:
			struct DirectionalLightData : public Light::BackedLightData {
				glm::vec3 mDirection;
				Lights::Shadows::CascadedShadowMap mShadowMap;
				float CalculateSphereOfInfluence() const override;
				void GenerateShadowMap();
				void RenderShadowsMap(glm::mat4 camview, const std::function<void(Core::Graphics::ShaderProgram*)>& rend_func);
				void SetUniforms(const Asset<Core::Graphics::ShaderProgram>& shader);

			};
			DirectionalLight(const std::weak_ptr<Core::Object>& parent);
		private:
		};
	}
}

#endif