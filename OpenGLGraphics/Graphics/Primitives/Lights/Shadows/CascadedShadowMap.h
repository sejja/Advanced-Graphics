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
#include "Graphics/Primitives/Lights/DirectionalLight.h"

namespace Graphics {
	namespace Primitives {
		namespace Lights {
			namespace Shadows {
				class CascadedShadowMap {
					DONTDISCARD std::vector<glm::vec4> GetFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view) const;
					DONTDISCARD glm::mat4 GetLightProjection(float zoom, const glm::u16vec2& dim, float nearplane, float farplane) const noexcept;
					DONTDISCARD glm::mat4 GetLightView(DirectionalLight& light) const noexcept;
					DONTDISCARD glm::vec3 GetFrustrumCenter(DirectionalLight& light) const;
					DONTDISCARD glm::mat4 OrthographicCoordinates(DirectionalLight& light) const;
				};
			}
		}
	}
}

#endif