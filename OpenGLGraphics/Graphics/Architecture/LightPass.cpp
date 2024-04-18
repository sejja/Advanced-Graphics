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
#include "Graphics/Architecture/Utils/GLUtils.h"

namespace Graphics {
	namespace Architecture {
		std::unordered_map<std::size_t, std::shared_ptr<::Graphics::Primitives::DirectionalLight::DirectionalLightData>> LightPass::sDirectionalLightData;
		std::unordered_map<std::size_t, std::shared_ptr<::Graphics::Primitives::SpotLight::SpotLightData>> LightPass::sSpotLightData;
		std::unordered_map<std::size_t, std::shared_ptr<::Graphics::Primitives::PointLight::PointLightData>> LightPass::sPointLightData;

		// ------------------------------------------------------------------------
		/*! Constructor
		*
		*   Constructs a Light Pass Pipeline
		*/ //----------------------------------------------------------------------
		LightPass::LightPass() {
			Core::Assets::ResourceManager& resmg = Singleton<Core::Assets::ResourceManager>::Instance();
			mLightSphere = resmg.GetResource<::Graphics::Primitives::GLBModel>("Content/Meshes/sphere_20_averaged.obj");
			mLightSphereShader = resmg.GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/ForwardRender.shader");
			mDirectionalShader = resmg.GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/DeferredDirectionalLighting.shader");
			mPointShader = resmg.GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/DeferredPointLighting.shader");
			mSpotShader = resmg.GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/DeferredSpotLighting.shader");
			mShadowShader = resmg.GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/Shadow.shader");
		}

		// ------------------------------------------------------------------------
		/*! Render Shadow Maps
		*
		*   Renders the shadows of every object
		*/ //----------------------------------------------------------------------
		void LightPass::RenderShadowMaps(const glm::u16vec2 dim, const glm::mat4& camview, const std::function<void(Core::Graphics::ShaderProgram*)>& rend_func) const {
			glCullFace(GL_BACK);
			glViewport(0, 0, dim.x, dim.y);

			//Renders all Shadow maps from directional lighting
			for (const auto& x : sDirectionalLightData)
				x.second->RenderShadowsMap(camview, rend_func);

			Core::Graphics::ShaderProgram* shadow = mShadowShader->Get();
			shadow->Bind();

			//Renders all Shadow maps from spot lighting
			for (const auto& x : sSpotLightData) {
				Core::Graphics::FrameBuffer& shmp = x.second->mShadowMap;
				glm::mat4 lightProjection = glm::perspective(glm::radians(120.f), 1.33f, 2.f, 2000.f);
				glm::mat4 lightView = glm::lookAt(x.second->mPosition, x.second->mDirection, glm::vec3(0, 1, 0));

				shmp.Bind();
				shmp.Clear(true);
				shadow->SetShaderUniform("uProjection", &lightProjection);
				shadow->SetShaderUniform("uView", &lightView);
				rend_func(shadow);
				shmp.Unbind();
				x.second->mShadowMatrix = lightProjection * lightView;
			}
		}

		// ------------------------------------------------------------------------
		/*! Render Lights
		*
		*   Using the buffers created on the geometry pass, we can
		*		compute the lighting for each pixel
		*/ //----------------------------------------------------------------------
		void LightPass::RenderLights(const glm::u16vec2 dim, const GBuffer& gBuffer) const {
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
			glViewport(0, 0, dim.x, dim.y);
			glEnable(GL_STENCIL_TEST);
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);

			auto shadptr = mPointShader->Get();
			const std::string id = "uLight";

			//For every point light
			for (const auto& x : sPointLightData) {
				StencilPass(x.second->mPosition, x.second->CalculateSphereOfInfluence());
				shadptr->Bind();
				shadptr->SetShaderUniform((id + ".mPosition").c_str(), &x.second->mPosition);
				shadptr->SetShaderUniform((id + ".mColor").c_str(), &x.second->mColor);
				shadptr->SetShaderUniform((id + ".mRadius").c_str(), &x.second->mRadius);
				shadptr->SetShaderUniform((id + ".mFallOff").c_str(), &x.second->mFallOff);
				glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
				Utils::GLUtils::RenderScreenQuad();
			}

			shadptr = mSpotShader->Get();

			//for every spotlight
			for (const auto& x : sSpotLightData) {
				StencilPass(x.second->mPosition, x.second->CalculateSphereOfInfluence());
				shadptr->Bind();
				shadptr->SetShaderUniform((id + ".mPosition").c_str(), &x.second->mPosition);
				shadptr->SetShaderUniform((id + ".mDirection").c_str(), &x.second->mDirection);
				shadptr->SetShaderUniform((id + ".mColor").c_str(), &x.second->mColor);
				shadptr->SetShaderUniform((id + ".mRadius").c_str(), &x.second->mRadius);
				shadptr->SetShaderUniform((id + ".mInnerAngle").c_str(), &x.second->mInner);
				shadptr->SetShaderUniform((id + ".mOutterAngle").c_str(), &x.second->mOutter);
				shadptr->SetShaderUniform((id + ".mFallOff").c_str(), &x.second->mFallOff);
				
				//avoid uploading the shadow texture if it doesn't cast shadows
				if (x.second->mShadowCaster) {
					shadptr->SetShaderUniform("uShadowMatrix", &x.second->mShadowMatrix);
					x.second->mShadowMap.BindTexture(4);
				}
				glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
				Utils::GLUtils::RenderScreenQuad();
			}

			glDisable(GL_STENCIL_TEST);

			shadptr = mDirectionalShader->Get();
			shadptr->Bind();

			//Render directional lights, without stencil
			for (const auto& x : sDirectionalLightData) {
				glm::vec3 dir = glm::normalize(x.second->mDirection);
				shadptr->SetShaderUniform((id + ".mDirection").c_str(), &dir);
				shadptr->SetShaderUniform((id + ".mColor").c_str(), &x.second->mColor);
				x.second->SetUniforms(mDirectionalShader);
				Utils::GLUtils::RenderScreenQuad();
			}

			glDisable(GL_BLEND);
		}

		// ------------------------------------------------------------------------
		/*! Stencil Pass
		*
		*   Delimits the zone of influence of a light in screen-space, avoiding drawing
		*	unnafected pixels by light
		*/ //----------------------------------------------------------------------
		void LightPass::StencilPass(const glm::vec3& pos, const float sphere) const {
			glClear(GL_STENCIL_BUFFER_BIT);
			glStencilFunc(GL_ALWAYS, 0, 0);
			glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
			glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
			glm::mat4 matrix = glm::translate(glm::mat4(1.0f), pos) *
				glm::rotate(glm::mat4(1.0f), 0.f, glm::vec3(0.0f, 0.0f, 1.0f)) *
				glm::rotate(glm::mat4(1.0f), 0.f, glm::vec3(1.0f, 0.0f, 0.0f)) *
				glm::rotate(glm::mat4(1.0f), 0.f, glm::vec3(0.0f, 1.0f, 0.0f)) *
				glm::scale(glm::mat4(1.0f), glm::vec3(sphere, sphere, sphere));
			Core::Graphics::ShaderProgram* light = mLightSphereShader->Get();
			light->Bind();
			light->SetShaderUniform("uModel", &matrix);
			mLightSphere->Get()->Draw();
		}
	}
}