//
//	CascadedShadowMap.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 18/03/24
//	Copyright � 2024 . All Rights reserved
//

#include "CascadedShadowMap.h"
#include "Graphics/Primitives/Lights/DirectionalLight.h"
#include "gtc/matrix_transform.hpp"
#include "Graphics/Camera.h"
#include "Graphics/Architecture/InstancedRendering/InstancedRendering.h"

namespace Graphics {
	namespace Primitives {
		namespace Lights {
			namespace Shadows {
                // ------------------------------------------------------------------------
                /*! Default Constructor
                *
                *   Constructs a Shadow Map, initializing the 3D Lightmaps and the Framebuffer
                */ //----------------------------------------------------------------------
                CascadedShadowMap::CascadedShadowMap() :
                    mCascadedLevels({ 10000.f / 50.0f, 10000.f / 25.0f, 10000.f / 10.0f, 10000.f / 2.0f }),
                    mShader(Singleton<Core::Assets::ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/CascadedShadowMap.shader")),
                    mInstancedShader(Singleton<Core::Assets::ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/InstancedShaders/CascadeShadow/InstancedCascadedShadowMap.shader")),
                    mLightMatrices() {
                    glGenFramebuffers(1, &mLightBuffer);

                    glGenTextures(1, &mLightMaps);
                    glBindTexture(GL_TEXTURE_2D_ARRAY, mLightMaps);
                    glTexImage3D(
                        GL_TEXTURE_2D_ARRAY,
                        0,
                        GL_DEPTH_COMPONENT32F,
                        c_cascaded_map_res,
                        c_cascaded_map_res,
                        GLsizei(mCascadedLevels.size()) + 1,
                        0,
                        GL_DEPTH_COMPONENT,
                        GL_FLOAT,
                        nullptr);

                    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
                    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

                    constexpr float bordercolor[] = { 0.f, 0.f, 0.f, 1.0f };
                    glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, bordercolor);

                    glBindFramebuffer(GL_FRAMEBUFFER, mLightBuffer);
                    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mLightMaps, NULL);
                    glDrawBuffer(GL_NONE);
                    glReadBuffer(GL_NONE);

                    // Always check that our framebuffer is ok
                    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                        throw CascadedShadowMapException("ERROR::FRAMEBUFFER::Framebuffer is not complete!");
                }

                // ------------------------------------------------------------------------
                /*! Get Light Space Matrix
                *
                *   Calculates the light matrix for each of the cascaded LODS
                */ //----------------------------------------------------------------------
                glm::mat4 CascadedShadowMap::GetLightSpaceMatrix(const glm::mat4& camview, const glm::vec3& lightdir, const float nearPlane, const float farPlane) const {
                    const glm::mat4 proj = glm::perspective(
                        glm::radians(45.f), (float)1600 / (float)900, nearPlane,
                        farPlane);
                    const std::array<glm::vec4, 8> corners = GetFrustrumCornersWorldSpace(proj  * camview);

                    glm::vec3 center = glm::vec3(0, 0, 0);
                    std::for_each_n(std::execution::par_unseq, corners.begin(), corners.size(), [&center](const glm::vec4& v) { center += glm::vec3(v); });

                    center /= corners.size();

                    const glm::mat4 lightView = glm::lookAt(center + lightdir, center, glm::vec3(0.0f, 1.0f, 0.0f));

                    float minX = std::numeric_limits<float>::max();
                    float maxX = std::numeric_limits<float>::lowest();
                    float minY = std::numeric_limits<float>::max();
                    float maxY = std::numeric_limits<float>::lowest();
                    float minZ = std::numeric_limits<float>::max();
                    float maxZ = std::numeric_limits<float>::lowest();

                    std::for_each(std::execution::unseq, corners.begin(), corners.end(), [&minX, &maxX, &minY, &maxY, &minZ, &maxZ, &lightView](const glm::vec4& v) {
						const glm::vec4 trf = lightView * v;
						minX = std::min(minX, trf.x);
						maxX = std::max(maxX, trf.x);
						minY = std::min(minY, trf.y);
						maxY = std::max(maxY, trf.y);
						minZ = std::min(minZ, trf.z);
						maxZ = std::max(maxZ, trf.z);
					});

                    // Tune this parameter according to the scene
                    constexpr float zMult = 20.0f;
                    if (minZ < 0)
                        minZ *= zMult;
                    else
                        minZ /= zMult;
                    if (maxZ < 0)
                        maxZ /= zMult;
                    else
                        maxZ *= zMult;

                    return glm::ortho(minX, maxX, minY, maxY, minZ, maxZ) * lightView;
                }

                 // ------------------------------------------------------------------------
                /*! Get Light Space Matrix
                *
                *   Calculates the light matrix for each of the cascaded LODS
                */ //----------------------------------------------------------------------
                std::array<glm::mat4, 5> CascadedShadowMap::GetLightSpaceMatrices(const glm::mat4& camview, const glm::vec3& lightdir) const {
                    std::array<glm::mat4, 5> ret{};

                    // Calculate the light space matrices for each of the cascaded LODS
                    for (size_t i = 0; i < mCascadedLevels.size() + 1; ++i) {
                        if (i == 0)
                            ret[i] = (GetLightSpaceMatrix(camview, lightdir, Core::Primitives::Camera::cnearPlane, mCascadedLevels[i]));
                        else if (i < mCascadedLevels.size())
                            ret[i] = (GetLightSpaceMatrix(camview, lightdir, mCascadedLevels[i - 1], mCascadedLevels[i]));
                        else
                            ret[i] = (GetLightSpaceMatrix(camview, lightdir, mCascadedLevels[i - 1], Core::Primitives::Camera::cfarPlane));
                    }
                    return ret;
                }

                // ------------------------------------------------------------------------
               /*! Bind
               *
               *   Binds the lightmaps to the shader
               */ //----------------------------------------------------------------------
                void CascadedShadowMap::Bind() const {
                    glActiveTexture(GL_TEXTURE4);
                    glBindTexture(GL_TEXTURE_2D_ARRAY, mLightMaps);
                }

                // ------------------------------------------------------------------------
                /*! Set Uniforms
                *
                *   Sets the uniforms for a given shader
                */ //----------------------------------------------------------------------
                void CascadedShadowMap::SetUniforms(const Core::Assets::Asset<Core::Graphics::ShaderProgram>& shader) {
                    const std::size_t cascadeLevels = mCascadedLevels.size();
                    Core::Graphics::ShaderProgram* const shd = shader->Get();
                    constexpr const char* uniformslightSpaces[] = { "lightSpaceMatrices[0]", "lightSpaceMatrices[1]", "lightSpaceMatrices[2]", "lightSpaceMatrices[3]", "lightSpaceMatrices[4]" };
                    constexpr const char* uniformsCascadeLevels[] = { "cascadePlaneDistances[0]", "cascadePlaneDistances[1]", "cascadePlaneDistances[2]", "cascadePlaneDistances[3]" };

                    Bind();

                    shd->SetShaderUniform("cascadeCount", &cascadeLevels);

                    // Set the light space matrices
                    for (size_t i = 0; i < mLightMatrices.size(); ++i)
                        shd->SetShaderUniform(uniformslightSpaces[i], &mLightMatrices[i]);
                    
                    // Set the cascade levels
                    for (size_t i = 0; i < mCascadedLevels.size(); ++i)
                        shd->SetShaderUniform(uniformsCascadeLevels[i], &mCascadedLevels[i]);
                }

                // ------------------------------------------------------------------------
                /*! Render
                *
                *   Renders the Shadow Depth Maps
                */ //----------------------------------------------------------------------
                void CascadedShadowMap::Render(const glm::mat4& camview, const glm::vec3& dir, const std::function<void(Core::Graphics::ShaderProgram*)>& rend_func) {
                    std::array<glm::mat4, 5> lightMatrices = GetLightSpaceMatrices(camview, dir);
                    constexpr const char* uniformslightSpaces[] = { "lightSpaceMatrices[0]", "lightSpaceMatrices[1]", "lightSpaceMatrices[2]", "lightSpaceMatrices[3]", "lightSpaceMatrices[4]" };

                    Core::Graphics::ShaderProgram* shd = mShader->Get();
                    mLightMatrices = lightMatrices;
                    shd->Bind();

                    // Set the light space matrices
                    for (size_t i = 0; i < mLightMatrices.size(); ++i)
                        shd->SetShaderUniform(uniformslightSpaces[i], &mLightMatrices[i]);

                    glBindFramebuffer(GL_FRAMEBUFFER, mLightBuffer);
                    glViewport(0, 0, c_cascaded_map_res, c_cascaded_map_res);
                    glClear(GL_DEPTH_BUFFER_BIT);
                    glCullFace(GL_FRONT);  // peter panning
                    rend_func(shd);

                    //---------------------------------------------------------------------------

                    if (Singleton<Graphics::Architecture::InstancedRendering::InstanceRenderer>::Instance().has_instanced())
                    {
                        shd = mInstancedShader->Get();
                        shd->Bind();

                        // Set the light space matrices
                        for (size_t i = 0; i < mLightMatrices.size(); ++i)
                            shd->SetShaderUniform(uniformslightSpaces[i], &mLightMatrices[i]);

                        Singleton<Graphics::Architecture::InstancedRendering::InstanceRenderer>::Instance().render_shader(0, shd);
                    }
                    glCullFace(GL_BACK);
                }

                // ------------------------------------------------------------------------
                /*! Get Frustrum Corners World Space
                *
                *   Returns the corners of the frustrum in world space
                */ //----------------------------------------------------------------------
                std::array<glm::vec4, 8> CascadedShadowMap::GetFrustrumCornersWorldSpace(const glm::mat4& mtx) const  {
                    const auto inv = glm::inverse(mtx);

                    std::array<glm::vec4, 8> frustumCorners;
                    int i = 0;
                    for (unsigned char x = 0; x < 2; ++x)
                        for (unsigned char y = 0; y < 2; ++y)
                            for (unsigned char z = 0; z < 2; ++z) {
                                const glm::vec4 pt = inv * glm::vec4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f);
                                frustumCorners[i++] = (pt / pt.w);
                            }

                    return frustumCorners;
                }
			}
		}
	}
}