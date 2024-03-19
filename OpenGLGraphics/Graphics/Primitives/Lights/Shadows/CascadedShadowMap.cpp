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
                */ // ---------------------------------------------------------------------
                std::vector<glm::vec4> CascadedShadowMap::GetFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view) const {
                    const auto inv = glm::inverse(proj * view);

                    std::vector<glm::vec4> frustumCorners;

                    //Left-Right
                    for (unsigned int x = 0; x < 2; ++x) {
                        //Up-Down
                        for (unsigned int y = 0; y < 2; ++y) {
                            //Botom-Front
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

                // ------------------------------------------------------------------------
                /*! Get Light Projection
                *
                *   Calculate the projection matrix of a directional light
                */ // ---------------------------------------------------------------------
                glm::mat4 CascadedShadowMap::GetLightProjection(float zoom, const glm::u16vec2& dim, float nearplane, float farplane) const noexcept {
                    return  glm::perspective(glm::radians(zoom),
                        static_cast<float>(dim.x) / static_cast<float>(dim.y),
                        nearplane,
                        farplane
                    );
                }

                // ------------------------------------------------------------------------
                /*! Get Light View
                *
                *   Calculate the view matrix of a directional light
                */ // ---------------------------------------------------------------------
                glm::mat4 CascadedShadowMap::GetLightView(DirectionalLight& light) const noexcept {
                    return glm::lookAt(
                        light.mData->mPosition + reinterpret_cast<DirectionalLight::DirectionalLightData*>(light.mData)->mDirection,
                        light.mData->mPosition,
                        glm::vec3(0.0f, 1.0f, 0.0f)
                    );
                }
                
                // ------------------------------------------------------------------------
                /*! Get Frustrum Center
                *
                *   Returns the Center of the Frustrum
                */ // ---------------------------------------------------------------------
                glm::vec3 CascadedShadowMap::GetFrustrumCenter(DirectionalLight& light) const {
                    glm::vec3 center = glm::vec3(0, 0, 0);
                    auto corners = GetFrustumCornersWorldSpace(
                        GetLightProjection(1, { 1600, 900 }, 0.1f, 10000.f),
                        GetLightView(light));

                    //Weight every vertex
                    for (const auto& v : corners) {
                        center += glm::vec3(v);
                    }
                    center /= corners.size();

                    return center;
                }

                glm::mat4 CascadedShadowMap::OrthographicCoordinates(DirectionalLight& light) const {
                    float minX = std::numeric_limits<float>::max();
                    float maxX = std::numeric_limits<float>::lowest();
                    float minY = std::numeric_limits<float>::max();
                    float maxY = std::numeric_limits<float>::lowest();
                    float minZ = std::numeric_limits<float>::max();
                    float maxZ = std::numeric_limits<float>::lowest();
                    auto corners = GetFrustumCornersWorldSpace(
                        GetLightProjection(1, { 1600, 900 }, 0.1f, 10000.f),
                        GetLightView(light));
                    auto lightView = GetLightView(light);

                    for (const auto& v : corners) {
                        const auto trf = lightView * v;
                        minX = std::min(minX, trf.x);
                        maxX = std::max(maxX, trf.x);
                        minY = std::min(minY, trf.y);
                        maxY = std::max(maxY, trf.y);
                        minZ = std::min(minZ, trf.z);
                        maxZ = std::max(maxZ, trf.z);
                    }

                    //We need to tweak this MAGIC value
                    constexpr float zMult = 1000.f;
                    if (minZ < 0)
                        minZ *= zMult;
                    else
                        minZ /= zMult;

                    if (maxZ < 0)
                        maxZ /= zMult;
                    else
                        maxZ *= zMult;

                    const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);

                    return lightProjection * lightView;
                }
			}
		}
	}
}