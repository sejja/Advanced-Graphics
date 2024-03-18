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
#include "CommonDefines.h"

namespace Graphics {
	namespace Primitives {
		namespace Lights {
			namespace Shadows {
				class CascadedShadowMap {
					DONTDISCARD std::vector<glm::vec4> GetFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view);
				};
			}
		}
	}
}

#endif