//
//	CascadedShadowMap.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 18/03/24
//	Copyright � 2024 . All Rights reserved
//


#ifndef _CASCADED_SHADOW_MAP_H_
#define _CASCADED_SHADOW_MAP_H_

#include <vector>
#include <glm.hpp>
#include "gtc/matrix_transform.hpp"
#include "CommonDefines.h"
#include "Graphics/Primitives/ShaderProgram.h"
#include "Core/ResourceManager.h"

namespace Graphics {
	namespace Primitives {

		class DirectionalLight;

		namespace Lights {
			namespace Shadows {
				class CascadedShadowMap {
				public:
					CascadedShadowMap();
					void CreateShadowMapGPUData();
					void Bind();
					void SetUniforms(const Asset<Core::Graphics::ShaderProgram>& shader);
					void Render(glm::mat4 camview, glm::vec3 pos, glm::vec3 dir, const std::function<void(Core::Graphics::ShaderProgram*)>& rend_func);
					std::vector<glm::mat4> GetLightSpaceMatrices(glm::mat4 camview, glm::vec3 lightdir);
					glm::mat4 GetLightSpaceMatrix(glm::mat4 camview, glm::vec3 lightdir, const float nearPlane, const float farPlane);
					std::vector<glm::vec4> GetFrustrumCornersWorldSpace(glm::mat4 mtx);
				private:
					GLuint lightFBO, lightDepthMaps;
					GLuint matricesUBO;
					float depthMapResolution = 4096;
					std::vector<float> shadowCascadeLevels;
					std::vector<glm::mat4> mLightMatrices;
					Asset<Core::Graphics::ShaderProgram> shadowMapShader;
				};
			}
		}
	}
}

#endif