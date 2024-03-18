//
//	CascadedShadowMap.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 18/03/24
//	Copyright � 2024 . All Rights reserved
//

#include "CascadedShadowMap.h"

namespace Graphics {
	namespace Primitives {
		namespace Lights {
			namespace Shadows {
                // ------------------------------------------------------------------------
                /*! Get Frustum Corners World Space
                *
                *   Calculate the Corners of the frustrum in world space
                */ // --------------------------------------------------------------------
                std::vector<glm::vec4> CascadedShadowMap::GetFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view) {
                    const auto inv = glm::inverse(proj * view);

                    std::vector<glm::vec4> frustumCorners;
                    for (unsigned int x = 0; x < 2; ++x) {
                        for (unsigned int y = 0; y < 2; ++y) {
                            for (unsigned int z = 0; z < 2; ++z) {
                                const glm::vec4 pt =
                                    inv * glm::vec4(
                                        2.0f * x - 1.0f,
                                        2.0f * y - 1.0f,
                                        2.0f * z - 1.0f,
                                        1.0f);
                                frustumCorners.push_back(pt / pt.w);
                            }
                        }
                    }

                    return frustumCorners;
                }
			}
		}
	}
}