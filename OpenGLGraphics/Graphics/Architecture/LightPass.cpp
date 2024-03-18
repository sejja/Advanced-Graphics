//	LightPass.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 13/03/24
//	Copyright � 2024. All Rights reserved
//

#include "LightPass.h"
#include "Graphics/Primitives/Light.h"
#include "gtc/matrix_transform.hpp"

namespace Graphics {
	namespace Architecture {
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
		}
		
		LightPass::~LightPass() {
			glDeleteVertexArrays(1, &mScreenQuadVAO);
			glDeleteBuffers(1, &mScreenQuadVBO);
		}

		// ------------------------------------------------------------------------
		/*! Lighting Pass
		*
		*   Using the buffers created on the geometry pass, we can
		*		compute the lighting for each pixel
		*/ //----------------------------------------------------------------------
		void LightPass::RenderLights(Core::Graphics::GBuffer& gBuffer, Bloom::BloomRenderer& bloomRend, std::vector<glm::mat4>& shadow_mtx) {
			glBindFramebuffer(GL_FRAMEBUFFER, NULL);
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
			gBuffer.BindLightingShader();
			glViewport(0, 0, 1600, 900);
			glEnable(GL_STENCIL_TEST);

			for (int i = 0; i < ::Graphics::Primitives::Light::sLightReg; i++) {
				StencilPass(::Graphics::Primitives::Light::sLightData[i]);
				auto shadptr = gBuffer.GetLightingShader()->Get();
				const std::string id = "uLight";
				shadptr->Bind();
				shadptr->SetShaderUniform((id + ".mPosition").c_str(), &::Graphics::Primitives::Light::sLightData[i].mPosition);
				shadptr->SetShaderUniform((id + ".mDirection").c_str(), &::Graphics::Primitives::Light::sLightData[i].mDirection);
				shadptr->SetShaderUniform((id + ".mColor").c_str(), &::Graphics::Primitives::Light::sLightData[i].mColor);
				shadptr->SetShaderUniform((id + ".mRadius").c_str(), &::Graphics::Primitives::Light::sLightData[i].mRadius);
				shadptr->SetShaderUniform((id + ".mInnerAngle").c_str(), &::Graphics::Primitives::Light::sLightData[i].mInner);
				shadptr->SetShaderUniform((id + ".mOutterAngle").c_str(), &::Graphics::Primitives::Light::sLightData[i].mOutter);
				shadptr->SetShaderUniform((id + ".mFallOff").c_str(), &::Graphics::Primitives::Light::sLightData[i].mFallOff);
				shadptr->SetShaderUniform((id + ".mType").c_str(), static_cast<int>(::Graphics::Primitives::Light::sLightData[i].mType));
				shadptr->SetShaderUniform((id + ".mCastShadows").c_str(), static_cast<int>(::Graphics::Primitives::Light::sLightData[i].mShadowCaster));
				if (::Graphics::Primitives::Light::sLightData[i].mShadowCaster) {
					shadptr->SetShaderUniform("uShadowMatrix", shadow_mtx.data() + i);
					::Graphics::Primitives::Light::sLightData[i].mShadowMap.BindTexture(4);
				}
				glEnable(GL_CULL_FACE);
				RenderScreenQuad();
				glCullFace(GL_BACK);

			}
		}

		void LightPass::StencilPass(Primitives::Light::BackedLightData& data) {
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			glClear(GL_STENCIL_BUFFER_BIT);
			glStencilFunc(GL_ALWAYS, 0, 0);
			glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
			glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
			float radius = data.CalculateSphereOfInfluence();
			glm::mat4 matrix = glm::translate(glm::mat4(1.0f), data.mPosition) *
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