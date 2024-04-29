//	LightPass.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 13/03/24
//	Copyright � 2024. All Rights reserved
//

#include "LightPass.h"
#include "Graphics/Primitives/Lights/Light.h"
#include "gtc/matrix_transform.hpp"
#include "Graphics/Primitives/ShaderProgram.h"

namespace Graphics {
	namespace Architecture {

		std::unordered_map<std::size_t, ::Graphics::Primitives::DirectionalLight::DirectionalLightData*> LightPass::sDirectionalLightData;
		std::unordered_map<std::size_t, ::Graphics::Primitives::SpotLight::SpotLightData*> LightPass::sSpotLightData;
		std::unordered_map<std::size_t, ::Graphics::Primitives::PointLight::PointLightData*> LightPass::sPointLightData;

		LightPass::LightPass() {
			float quadVertices[] = {
				// positions        // texture Coords
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			};
			// setup plane VAO
			glGenVertexArrays(1, &mScreenQuadVAO);
			glGenBuffers(1, &mScreenQuadVAO);
			glBindVertexArray(mScreenQuadVBO);
			glBindBuffer(GL_ARRAY_BUFFER, mScreenQuadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			mLightSphere = Singleton<ResourceManager>::Instance().GetResource<::Graphics::Primitives::GLBModel>("Content/Meshes/sphere_20_averaged.obj");
			mLightSphereShader = Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/ForwardRender.shader");
			mDirectionalShader = Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/DeferredDirectionalLighting.shader");
			mPointShader = Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/DeferredPointLighting.shader");
			mSpotShader = Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/DeferredSpotLighting.shader");
		}

		LightPass::~LightPass() {
			glDeleteVertexArrays(1, &mScreenQuadVAO);
			glDeleteBuffers(1, &mScreenQuadVBO);
		}

		void LightPass::RenderShadowMaps(glm::mat4 camview, const std::function<void(Core::Graphics::ShaderProgram*)>& rend_func) {
			std::vector<glm::mat4> shadow_matrices;

			for (auto& x : sDirectionalLightData) {
				glCullFace(GL_NONE);
				x.second->RenderShadowsMap(camview, rend_func);
			}

			glViewport(0, 0, 1600, 900);
			for (auto& x : sSpotLightData) {
				if (!x.second->mShadowCaster) continue;
				x.second->mShadowMap.Bind();
				x.second->mShadowMap.Clear(true);

				auto up = glm::normalize(glm::cross(glm::cross(-x.second->mPosition, glm::vec3(0, 1, 0)), -x.second->mPosition));
				glm::mat4 lightProjection = glm::perspective(glm::radians(120.f), 1.33f, 2.f, 2000.f);
				glm::mat4 lightView = glm::lookAt(x.second->mPosition, x.second->mDirection, glm::vec3(0, 1, 0));
				glm::mat4 shadow_matrix = lightProjection * lightView;
				shadow_matrices.push_back(shadow_matrix);

				{
					const auto shadow = Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/Shadow.shader")->Get();
					shadow->Bind();
					shadow->SetShaderUniform("uProjection", &lightProjection);
					shadow->SetShaderUniform("uView", &lightView);
					glCullFace(GL_NONE);
					rend_func(shadow);
				}

				x.second->mShadowMap.Unbind();
				x.second->mShadowMatrix = shadow_matrix;
			}

			glViewport(0, 0, 1024, 1024); //Creo

			float near = 1.0f;
			float far = 25.0f;
			glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, near, far);

			for (auto& x : sPointLightData) {
				auto lightData = x.second;

				std::vector<glm::mat4> shadowTransforms;
				shadowTransforms.push_back(shadowProj * glm::lookAt(lightData->mPosition, lightData->mPosition + glm::vec3(1, 0, 0), glm::vec3(0, -1, 0))); //falta multiplicar por la proyección
				shadowTransforms.push_back(shadowProj * glm::lookAt(lightData->mPosition, lightData->mPosition + glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)));
				shadowTransforms.push_back(shadowProj * glm::lookAt(lightData->mPosition, lightData->mPosition + glm::vec3(0, 1, 0), glm::vec3(0, 0, 1))); 
				shadowTransforms.push_back(shadowProj* glm::lookAt(lightData->mPosition, lightData->mPosition + glm::vec3(0, -1, 0), glm::vec3(0, 0, -1)));
				shadowTransforms.push_back(shadowProj* glm::lookAt(lightData->mPosition, lightData->mPosition + glm::vec3(1, 0, 1), glm::vec3(0, -1, 0)));
				shadowTransforms.push_back(shadowProj * glm::lookAt(lightData->mPosition, lightData->mPosition + glm::vec3(1, 0, -1), glm::vec3(0, -1, 0)));

				
				//Configurar shader
				auto shader = Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/PointShadow.shader")->Get();

				/*for (int i = 0; i < 6; i++) {
					shader->set
				}*/

				shader->Bind();
				shader->SetShaderUniform("uLight", &shadowTransforms[0]);
				shader->SetShaderUniform("lightPos", &lightData->mPosition);
				shader->SetShaderUniform("far_plane", far);

				lightData->depthMapFBO.Bind();
				lightData->depthMapFBO.Clear(true);
				rend_func(shader); //Renderizar
				lightData->depthMapFBO.Unbind();
			}
		}

		// ------------------------------------------------------------------------
		/*! Lighting Pass
		*
		*   Using the buffers created on the geometry pass, we can
		*		compute the lighting for each pixel
		*/ //----------------------------------------------------------------------
		void LightPass::RenderLights(Core::Graphics::GBuffer& gBuffer, Bloom::BloomRenderer& bloomRend) {
			//glBindFramebuffer(GL_FRAMEBUFFER, NULL);
			glEnable(GL_BLEND);
			glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_ONE, GL_ONE);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gBuffer.GetPositionTextureHandle());
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, gBuffer.GetNormalTextureHandle());
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, gBuffer.GetAlbedoTextureHandle());
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, bloomRend.BloomTexture());
			glViewport(0, 0, 1600, 900);
			glEnable(GL_STENCIL_TEST);

			auto shadptr = mDirectionalShader->Get();
			const std::string id = "uLight";

			for (auto& x : sPointLightData) {
				StencilPass(x.second->mPosition, x.second->CalculateSphereOfInfluence());
				glEnable(GL_BLEND);
				glBlendEquation(GL_FUNC_ADD);
				glBlendFunc(GL_ONE, GL_ONE);
				shadptr = mPointShader->Get();
				shadptr->Bind();
				shadptr->SetShaderUniform((id + ".mPosition").c_str(), &x.second->mPosition);
				shadptr->SetShaderUniform((id + ".mColor").c_str(), &x.second->mColor);
				shadptr->SetShaderUniform((id + ".mRadius").c_str(), &x.second->mRadius);
				shadptr->SetShaderUniform((id + ".mFallOff").c_str(), &x.second->mFallOff);
				//shadptr->SetShaderUniform((id + ".mCastShadows").c_str(), static_cast<int>(x.second->mShadowCaster));
				glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
				glEnable(GL_CULL_FACE);
				RenderScreenQuad();
				glCullFace(GL_BACK);
				glDisable(GL_BLEND);
			}

			for (auto& x : sSpotLightData) {
				StencilPass(x.second->mPosition, x.second->CalculateSphereOfInfluence());
				glEnable(GL_BLEND);
				glBlendEquation(GL_FUNC_ADD);
				glBlendFunc(GL_ONE, GL_ONE);
				auto shadptr = mSpotShader->Get();
				const std::string id = "uLight";
				shadptr->Bind();
				shadptr->SetShaderUniform((id + ".mPosition").c_str(), &x.second->mPosition);
				shadptr->SetShaderUniform((id + ".mDirection").c_str(), &x.second->mDirection);
				shadptr->SetShaderUniform((id + ".mColor").c_str(), &x.second->mColor);
				shadptr->SetShaderUniform((id + ".mRadius").c_str(), &x.second->mRadius);
				shadptr->SetShaderUniform((id + ".mInnerAngle").c_str(), &x.second->mInner);
				shadptr->SetShaderUniform((id + ".mOutterAngle").c_str(), &x.second->mOutter);
				shadptr->SetShaderUniform((id + ".mFallOff").c_str(), &x.second->mFallOff);
				shadptr->SetShaderUniform((id + ".mCastShadows").c_str(), static_cast<int>(x.second->mShadowCaster));
				if (x.second->mShadowCaster) {
					shadptr->SetShaderUniform("uShadowMatrix", &x.second->mShadowMatrix);
					x.second->mShadowMap.BindTexture(4);
				}
				glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
				glEnable(GL_CULL_FACE);
				RenderScreenQuad();
				glCullFace(GL_BACK);
				glDisable(GL_BLEND);
			}

			glDisable(GL_STENCIL_TEST);

			shadptr = mDirectionalShader->Get();
			shadptr->Bind();

			for (auto& x : sDirectionalLightData) {
				glEnable(GL_BLEND);
				glm::vec3 dir = glm::normalize(glm::vec3(20.0f, 50, 20.0f));
				shadptr->SetShaderUniform((id + ".mDirection").c_str(), &dir);
				shadptr->SetShaderUniform((id + ".mColor").c_str(), &x.second->mColor);
				dynamic_cast<::Graphics::Primitives::DirectionalLight::DirectionalLightData*>(x.second)->SetUniforms(mDirectionalShader);
				//shadptr->SetShaderUniform((id + ".mCastShadows").c_str(), static_cast<int>(x.second->mShadowCaster));
				glEnable(GL_CULL_FACE);
				RenderScreenQuad();
				glCullFace(GL_BACK);
				glDisable(GL_BLEND);
			}
		}

		void LightPass::StencilPass(glm::vec3& pos, float sphere) {
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			glClear(GL_STENCIL_BUFFER_BIT);
			glStencilFunc(GL_ALWAYS, 0, 0);
			glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
			glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
			float radius = sphere;
			glm::mat4 matrix = glm::translate(glm::mat4(1.0f), pos) *
				glm::rotate(glm::mat4(1.0f), 0.f, glm::vec3(0.0f, 0.0f, 1.0f)) *
				glm::rotate(glm::mat4(1.0f), 0.f, glm::vec3(1.0f, 0.0f, 0.0f)) *
				glm::rotate(glm::mat4(1.0f), 0.f, glm::vec3(0.0f, 1.0f, 0.0f)) *
				glm::scale(glm::mat4(1.0f), glm::vec3(radius, radius, radius));
			mLightSphereShader->Get()->Bind();
			mLightSphereShader->Get()->SetShaderUniform("uModel", &matrix);
			mLightSphere->Get()->Draw(*mLightSphereShader->Get());
		}

		unsigned int quadVAO = 0;
		unsigned int quadVBO;
		void LightPass::RenderScreenQuad() {
			if (quadVAO == 0)
			{
				float quadVertices[] = {
					// positions        // texture Coords
					-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
					-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
					 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
					 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
				};
				// setup plane VAO
				glGenVertexArrays(1, &quadVAO);
				glGenBuffers(1, &quadVBO);
				glBindVertexArray(quadVAO);
				glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			}
			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glBindVertexArray(0);
		}
	}
}