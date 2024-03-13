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
#include "Graphics/Primitives/Light.h"

namespace Graphics {
	namespace Architecture {
		class LightPass {
		public:
			LightPass();
			~LightPass();

			void RenderLights(Core::Graphics::GBuffer& gBuffer, Bloom::BloomRenderer& bloomRend, const std::vector<glm::mat4>& shadow_mtx);
			void RenderScreenQuad();
			void StencilPass(Primitives::Light::BackedLightData& data);
			GLuint mScreenQuadVAO, mScreenQuadVBO;
			Asset<::Graphics::Primitives::GLBModel> mLightSphere;
			Asset<Core::Graphics::ShaderProgram> mLightSphereShader;
		};
	}
}

#endif