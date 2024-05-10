//
//	CascadedShadowMap.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 18/03/24
//	Copyright � 2024 . All Rights reserved
//


#ifndef _CASCADED_SHADOW_MAP_H_
#define _CASCADED_SHADOW_MAP_H_

#include <array>
#include <glm.hpp>
#include "CommonDefines.h"
#include "Graphics/Primitives/ShaderProgram.h"
#include "Core/Assets/ResourceManager.h"

namespace Graphics {
	namespace Primitives {
		namespace Lights {
			namespace Shadows {
				constexpr GLsizei c_cascaded_map_res = 4096;

				class CascadedShadowMap {
				#pragma region //Declarations
					CLASS_EXCEPTION(CascadedShadowMap)
				#pragma endregion

				#pragma region //Constructor
				public:
					CascadedShadowMap();
				#pragma endregion

				#pragma region //Methods
					void Bind() const;
					void SetUniforms(const Core::Assets::Asset<Core::Graphics::ShaderProgram>& shader);
					void Render(const glm::mat4& camview, const glm::vec3& dir, const std::function<void(Core::Graphics::ShaderProgram*)>& rend_func);
				
				private:
					DONTDISCARD std::array<glm::mat4, 5> GetLightSpaceMatrices(const glm::mat4& camview, const glm::vec3& lightdir) const;
					DONTDISCARD glm::mat4 GetLightSpaceMatrix(const glm::mat4& camview, const glm::vec3& lightdir, const float nearPlane, const float farPlane) const;
					DONTDISCARD std::array<glm::vec4, 8> GetFrustrumCornersWorldSpace(const glm::mat4& mtx) const;
				#pragma endregion

				#pragma region //Members
					GLuint mLightBuffer, mLightMaps;
					std::array<float, 4> mCascadedLevels;
					std::array<glm::mat4, 5> mLightMatrices;
					Core::Assets::Asset<Core::Graphics::ShaderProgram> mShader;
					Core::Assets::Asset<Core::Graphics::ShaderProgram> mInstancedShader;
				#pragma endregion
				};
			}
		}
	}
}

#endif