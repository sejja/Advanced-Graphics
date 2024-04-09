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
#include "Core/Assets/ResourceManager.h"
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

			void RenderShadowMaps(glm::mat4 camview, const std::function<void(Core::Graphics::ShaderProgram*)>& rend_func);
			void RenderLights(Core::Graphics::GBuffer& gBuffer, Bloom::BloomRenderer& bloomRend);
			void RenderScreenQuad();
			void StencilPass(glm::vec3& pos, float sphere);
			GLuint mScreenQuadVAO, mScreenQuadVBO;
			Core::Assets::Asset<::Graphics::Primitives::GLBModel> mLightSphere;
			Core::Assets::Asset<Core::Graphics::ShaderProgram> mLightSphereShader;
			Core::Assets::Asset<Core::Graphics::ShaderProgram> mDirectionalShader;
			Core::Assets::Asset<Core::Graphics::ShaderProgram> mPointShader;
			Core::Assets::Asset<Core::Graphics::ShaderProgram> mSpotShader;
			static std::unordered_map<std::size_t, Graphics::Primitives::DirectionalLight::DirectionalLightData*> sDirectionalLightData;
			static std::unordered_map<std::size_t, Graphics::Primitives::SpotLight::SpotLightData*> sSpotLightData;
			static std::unordered_map<std::size_t, Graphics::Primitives::PointLight::PointLightData*> sPointLightData;
		};
	}
}

#endif