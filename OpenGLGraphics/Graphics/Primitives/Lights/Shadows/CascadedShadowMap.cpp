//
//	CascadedShadowMap.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 18/03/24
//	Copyright � 2024 . All Rights reserved
//

#include "CascadedShadowMap.h"
#include "Graphics/Primitives/Lights/DirectionalLight.h"
#include <iostream>
#include "gtc/matrix_transform.hpp"

namespace Graphics {
	namespace Primitives {
		namespace Lights {
			namespace Shadows {
                CascadedShadowMap::CascadedShadowMap() {
                    CreateShadowMapGPUData();
                    mShader = Singleton<Core::Assets::ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/CascadedShadowMap.shader");
                }
                void CascadedShadowMap::CreateShadowMapGPUData() {
                    mCascadedLevels = { 10000.f / 50.0f, 10000.f / 25.0f, 10000.f / 10.0f, 10000.f / 2.0f };
                    glGenFramebuffers(1, &mLightBuffer);

                    glGenTextures(1, &mLightMaps);
                    glBindTexture(GL_TEXTURE_2D_ARRAY, mLightMaps);
                    glTexImage3D(
                        GL_TEXTURE_2D_ARRAY,
                        0,
                        GL_DEPTH_COMPONENT32F,
                        c_cascaded_map_res,
                        c_cascaded_map_res,
                        int(mCascadedLevels.size()) + 1,
                        0,
                        GL_DEPTH_COMPONENT,
                        GL_FLOAT,
                        nullptr);

                    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
                    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

                    constexpr float bordercolor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
                    glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, bordercolor);

                    glBindFramebuffer(GL_FRAMEBUFFER, mLightBuffer);
                    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mLightMaps, 0);
                    glDrawBuffer(GL_NONE);
                    glReadBuffer(GL_NONE);


                    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
                    if (status != GL_FRAMEBUFFER_COMPLETE)
                    {
                        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!";
                        throw 0;
                    }

                    glBindFramebuffer(GL_FRAMEBUFFER, 0);
                }

                glm::mat4 CascadedShadowMap::GetLightSpaceMatrix(const glm::mat4& camview, const glm::vec3& lightdir, const float nearPlane, const float farPlane) const
                {
                    const auto proj = glm::perspective(
                        glm::radians(45.f), (float)1600 / (float)900, nearPlane,
                        farPlane);
                    const auto corners = GetFrustrumCornersWorldSpace(proj  * camview);

                    glm::vec3 center = glm::vec3(0, 0, 0);
                    for (const auto& v : corners)
                    {
                        center += glm::vec3(v);
                    }
                    center /= corners.size();

                    const auto lightView = glm::lookAt(center + lightdir, center, glm::vec3(0.0f, 1.0f, 0.0f));

                    float minX = std::numeric_limits<float>::max();
                    float maxX = std::numeric_limits<float>::lowest();
                    float minY = std::numeric_limits<float>::max();
                    float maxY = std::numeric_limits<float>::lowest();
                    float minZ = std::numeric_limits<float>::max();
                    float maxZ = std::numeric_limits<float>::lowest();
                    for (const auto& v : corners)
                    {
                        const auto trf = lightView * v;
                        minX = std::min(minX, trf.x);
                        maxX = std::max(maxX, trf.x);
                        minY = std::min(minY, trf.y);
                        maxY = std::max(maxY, trf.y);
                        minZ = std::min(minZ, trf.z);
                        maxZ = std::max(maxZ, trf.z);
                    }

                    // Tune this parameter according to the scene
                    constexpr float zMult = 10.0f;
                    if (minZ < 0)
                    {
                        minZ *= zMult;
                    }
                    else
                    {
                        minZ /= zMult;
                    }
                    if (maxZ < 0)
                    {
                        maxZ /= zMult;
                    }
                    else
                    {
                        maxZ *= zMult;
                    }

                    const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
                    return lightProjection * lightView;
                }

                std::array<glm::mat4, 5> CascadedShadowMap::GetLightSpaceMatrices(const glm::mat4& camview, const glm::vec3& lightdir) const
                {
                    std::array<glm::mat4, 5> ret;
                    for (size_t i = 0; i < mCascadedLevels.size() + 1; ++i)
                    {
                        if (i == 0)
                        {
                            ret[i] = (GetLightSpaceMatrix(camview, lightdir, 0.1f, mCascadedLevels[i]));
                        }
                        else if (i < mCascadedLevels.size())
                        {
                            ret[i] = (GetLightSpaceMatrix(camview, lightdir, mCascadedLevels[i - 1], mCascadedLevels[i]));
                        }
                        else
                        {
                            ret[i] = (GetLightSpaceMatrix(camview, lightdir, mCascadedLevels[i - 1], 10000.f));
                        }
                    }
                    return ret;
                }

                void CascadedShadowMap::Bind() const {
                    glActiveTexture(GL_TEXTURE4);
                    glBindTexture(GL_TEXTURE_2D_ARRAY, mLightMaps);
                }

                void CascadedShadowMap::SetUniforms(const Core::Assets::Asset<Core::Graphics::ShaderProgram>& shader) {
                    Bind();
                    int cascadeLevels = mCascadedLevels.size();

                    shader->Get()->SetShaderUniform("cascadeCount", &cascadeLevels);
                    for (size_t i = 0; i < mLightMatrices.size(); ++i)
                        shader->Get()->SetShaderUniform("lightSpaceMatrices[" + std::to_string(i) + "]", &mLightMatrices[i]);
                    for (size_t i = 0; i < mCascadedLevels.size(); ++i)
                    {
                        shader->Get()->SetShaderUniform("cascadePlaneDistances[" + std::to_string(i) + "]", &mCascadedLevels[i]);
                    }
                }

                void CascadedShadowMap::Render(glm::mat4 camview, glm::vec3 pos, glm::vec3 dir, const std::function<void(Core::Graphics::ShaderProgram*)>& rend_func) {
                    std::array<glm::mat4, 5> lightMatrices = GetLightSpaceMatrices(camview, dir);
                    /*glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
                    for (size_t i = 0; i < lightMatrices.size(); ++i)
                    {
                        glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof(glm::mat4x4), sizeof(glm::mat4x4), &lightMatrices[i]);
                    }
                    glBindBuffer(GL_UNIFORM_BUFFER, 0);*/
                    mLightMatrices = lightMatrices;
                    mShader->Get()->Bind();
                    for (size_t i = 0; i < lightMatrices.size(); ++i)
                        mShader->Get()->SetShaderUniform("lightSpaceMatrices[" + std::to_string(i) + "]", &lightMatrices[i]);

                    glBindFramebuffer(GL_FRAMEBUFFER, mLightBuffer);
                    glViewport(0, 0, c_cascaded_map_res, c_cascaded_map_res);
                    glClear(GL_DEPTH_BUFFER_BIT);
                    glCullFace(GL_FRONT);  // peter panning
                    rend_func(mShader->Get());
                    glCullFace(GL_BACK);
                    glBindFramebuffer(GL_FRAMEBUFFER, 0);
                }

                std::array<glm::vec4, 8> CascadedShadowMap::GetFrustrumCornersWorldSpace(const glm::mat4& mtx) const
                {
                    const auto inv = glm::inverse(mtx);

                    std::array<glm::vec4, 8> frustumCorners;
                    int i = 0;
                    for (unsigned int x = 0; x < 2; ++x)
                    {
                        for (unsigned int y = 0; y < 2; ++y)
                        {
                            for (unsigned int z = 0; z < 2; ++z)
                            {
                                const glm::vec4 pt = inv * glm::vec4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f);
                                frustumCorners[i++] = (pt / pt.w);
                            }
                        }
                    }

                    return frustumCorners;
                }
			}
		}
	}
}