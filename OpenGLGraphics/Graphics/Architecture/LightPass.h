//	LightPass.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 13/03/24
//	Copyright � 2024. All Rights reserved
//

#ifndef _LIGHT_PASS__H_
#define _LIGHT_PASS__H_

#include <glew.h>
#include <vector>
#include <glm.hpp>
#include "Graphics/Architecture/GBuffer.h"
#include "Bloom/BloomRenderer.h"
#include "Graphics/Primitives/GLBModel.h"
#include "Graphics/Primitives/ShaderProgram.h"
#include "Core/ResourceManager.h"
#include "Graphics/Primitives/Lights/Light.h"
#include "Graphics/Primitives/Lights/DirectionalLight.h"
#include "Graphics/Primitives/Lights/SpotLight.h"
#include "Graphics/Primitives/Lights/PointLight.h"
#include <functional>

namespace Graphics {
	namespace Architecture {
		class LightPass {
		public:
			LightPass();
			~LightPass();

			void RenderShadowMaps(const std::function<void(Core::Graphics::ShaderProgram*)>& rend_func);
			void RenderLights(Core::Graphics::GBuffer& gBuffer, Bloom::BloomRenderer& bloomRend);
			void RenderScreenQuad();
			void StencilPass(glm::vec3& pos, float sphere);
			GLuint mScreenQuadVAO, mScreenQuadVBO;
			Asset<::Graphics::Primitives::GLBModel> mLightSphere;
			Asset<Core::Graphics::ShaderProgram> mLightSphereShader;
			Asset<Core::Graphics::ShaderProgram> mDirectionalShader;
			Asset<Core::Graphics::ShaderProgram> mPointShader;
			Asset<Core::Graphics::ShaderProgram> mSpotShader;
			static std::unordered_map<std::size_t, Graphics::Primitives::DirectionalLight::DirectionalLightData*> sDirectionalLightData;
			static std::unordered_map<std::size_t, Graphics::Primitives::SpotLight::SpotLightData*> sSpotLightData;
			static std::unordered_map<std::size_t, Graphics::Primitives::PointLight::PointLightData*> sPointLightData;
		};
	}
}

#endif