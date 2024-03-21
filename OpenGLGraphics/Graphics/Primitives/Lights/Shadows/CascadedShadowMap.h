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
					void Render(glm::vec3 pos, glm::vec3 dir, const std::function<void(Core::Graphics::ShaderProgram*)>& rend_func);
					DONTDISCARD std::vector<glm::vec4> GetFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view) const;
					DONTDISCARD glm::mat4 GetLightProjection(float zoom, const glm::u16vec2& dim, float nearplane, float farplane) const noexcept;
					DONTDISCARD glm::mat4 GetLightView(glm::vec3 pos, glm::vec3 dir) const noexcept;
					DONTDISCARD glm::vec3 GetFrustrumCenter(glm::vec3 pos, glm::vec3 dir) const;
					DONTDISCARD glm::mat4 OrthographicCoordinates(glm::vec3 pos, glm::vec3 dir) const;
				private:
					GLuint lightFBO, lightDepthMaps;
					float depthMapResolution = 2048;
					std::vector<float> shadowCascadeLevels;
					Asset<Core::Graphics::ShaderProgram> shadowMapShader;
				};
			}
		}
	}
}

#endif