//
//	OpenGLPipeline.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 14/01/23
//	Copyright � 2023. All Rights reserved
//

#include <execution>
#include "OpenGLPipeline.h"
#include "gtc/matrix_transform.hpp"
#include "Graphics/Camera.h"
#include "Graphics/Primitives/Texture.h"
#include "Core/InputManager.h"
#include "Graphics/Primitives/Light.h"
#include "Core/Singleton.h"
#include "Graphics/Primitives/Texture.h"
#include "Graphics/Primitives/Skybox.h"
#include "Dependencies/ImGui/imgui.h"
#include "Dependencies/ImGui/imgui_impl_opengl3.h"
#include "Dependencies/ImGui/imgui_impl_sdl2.h"

namespace Core {
	namespace Graphics {
		static Primitives::Camera cam;

		// ------------------------------------------------------------------------
		/*! Init
		*
		*   Sets the Initial OpenGL Settings to Draw
		*/ //----------------------------------------------------------------------
		void OpenGLPipeline::Init() {
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glFrontFace(GL_CCW);
			glDisable(GL_BLEND);
			glDisable(GL_STENCIL_TEST);
			glClearColor(0.f, 0.f, 0.f, 0.f);
			mShadowBuffers.emplace_back();
			mShadowBuffers.emplace_back();
			mShadowBuffers.emplace_back();
			mShadowBuffers.emplace_back();
			mShadowBuffers[0].Create();
			mShadowBuffers[0].CreateRenderTexture({mDimensions.x * 2, mDimensions.y * 2}, false);
			mShadowBuffers[1].Create();
			mShadowBuffers[1].CreateRenderTexture({ mDimensions.x * 2, mDimensions.y * 2 }, false);
			mShadowBuffers[2].Create();
			mShadowBuffers[2].CreateRenderTexture({ mDimensions.x * 2, mDimensions.y * 2 }, false);
			mShadowBuffers[3].Create();
			mShadowBuffers[3].CreateRenderTexture({ mDimensions.x * 2, mDimensions.y * 2 }, false);
			mGBuffer = std::make_unique<GBuffer>();
			mDirectionalLightShader = Singleton<ResourceManager>::Instance().GetResource<ShaderProgram>("Content/Shaders/DirectionalLight.shader");

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

			glGenBuffers(1, &mUniformBuffer);

			glBindBuffer(GL_UNIFORM_BUFFER, mUniformBuffer);
			glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			glBindBufferRange(GL_UNIFORM_BUFFER, 0, mUniformBuffer, 0, 2 * sizeof(glm::mat4) + sizeof(glm::vec3));
		}

		// ------------------------------------------------------------------------
		/*! PreRender
		*
		*   Clears the whole FrameBuffer
		*/ //----------------------------------------------------------------------
		void OpenGLPipeline::PreRender() {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		// ------------------------------------------------------------------------
		/*! Render
		*
		*   Renders every object in the scene
		*/ //----------------------------------------------------------------------
		void OpenGLPipeline::Render() {
			ImGui_ImplSDL2_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			if (ImGui::Begin("Deferred Rendering")) {
				ImGui::Image((ImTextureID)mGBuffer->GetPositionTextureHandle(),
					ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
				ImGui::SameLine();
				ImGui::Image((ImTextureID)mGBuffer->GetAlbedoTextureHandle(),
					ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
				ImGui::Image((ImTextureID)mGBuffer->GetNormalTextureHandle(),
					ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
			}
			ImGui::End();

			if (ImGui::Begin("Shadow Mapping")) {
				int i = 0;
				for (FrameBuffer& buff : mShadowBuffers) {
					ImGui::Image((ImTextureID)buff.GetTextureHandle(),
						ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
				
					if (i < 1) {
						ImGui::SameLine();
						i++;
					} else
						i = 0;
				}
			}
			ImGui::End();
			
			RenderShadowMaps();
			Skybox::sCurrentSky->UploadSkyboxCubeMap();
			UpdateUniformBuffers();
			GeometryPass();
			LightingPass();
			glEnable(GL_DEPTH_TEST);
			mGBuffer->BlitDepthBuffer();
			Skybox::sCurrentSky->Render(cam);

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		// ------------------------------------------------------------------------
		/*! Upload Light Data to GPU
		*
		*   Uploads the light data to the shader
		*/ //----------------------------------------------------------------------
		void OpenGLPipeline::UploadLightDataToGPU(const AssetReference<Core::Graphics::ShaderProgram>& shader) {
			Core::Graphics::ShaderProgram* shadptr = shader.lock()->Get();

			for (size_t i = 0; i < ::Graphics::Primitives::Light::sLightReg; i++) {
				const std::string id = "uLight[" + std::to_string(i);
			
				shadptr->SetShaderUniform((id + "].mPosition").c_str(), &::Graphics::Primitives::Light::sLightData[i].mPosition);
				shadptr->SetShaderUniform((id + "].mDirection").c_str(), &::Graphics::Primitives::Light::sLightData[i].mDirection);
				shadptr->SetShaderUniform((id + "].mAmbient").c_str(), &::Graphics::Primitives::Light::sLightData[i].mAmbient);
				shadptr->SetShaderUniform((id + "].mDiffuse").c_str(), &::Graphics::Primitives::Light::sLightData[i].mDiffuse);
				shadptr->SetShaderUniform((id + "].mSpecular").c_str(), &::Graphics::Primitives::Light::sLightData[i].mSpecular);
				shadptr->SetShaderUniform((id + "].mAttenuation").c_str(), &::Graphics::Primitives::Light::sLightData[i].mAttenuation);
				shadptr->SetShaderUniform((id + "].mInnerAngle").c_str(), &::Graphics::Primitives::Light::sLightData[i].mInner);
				shadptr->SetShaderUniform((id + "].mOutterAngle").c_str(), &::Graphics::Primitives::Light::sLightData[i].mOutter);
				shadptr->SetShaderUniform((id + "].mFallOff").c_str(), &::Graphics::Primitives::Light::sLightData[i].mFallOff);
				shadptr->SetShaderUniform((id + "].mType").c_str(), static_cast<int>(::Graphics::Primitives::Light::sLightData[i].mType));
			}

			shadptr->SetShaderUniform("uLightCount", static_cast<int>(::Graphics::Primitives::Light::sLightReg));
		}

		// ------------------------------------------------------------------------
		/*! Geometry Pass
		*
		*   Draws the geometry on the G-Buffer
		*/ //----------------------------------------------------------------------
		void OpenGLPipeline::GeometryPass() {
			std::unordered_multimap<Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Renderable>>::const_iterator> obsoletes;

			auto f_flushobosoletes = [this, &obsoletes]() {
				std::for_each(std::execution::par, obsoletes.begin(), obsoletes.end(), [this, &obsoletes](std::pair<const Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Renderable>>::const_iterator> x) {
					std::vector<std::weak_ptr<Renderable>>& it = mGroupedRenderables.find(x.first)->second;
					it.erase(x.second);

					//If we don't have any other renderables, erase it
					if (!it.size()) mGroupedRenderables.erase(x.first);
					});
				};

			auto f_grouprender = [&obsoletes](const std::pair<Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Renderable>>>& it, ShaderProgram* shader) {
				//For each renderable in shader program
				for (std::vector<std::weak_ptr<Renderable>>::const_iterator it2 = it.second.begin(); it2 != it.second.end(); it2++) {
					//If it isn't expired
					if (auto renderable = it2->lock()) {
						const std::shared_ptr<Object> parent = renderable->GetParent().lock();
						glm::mat4 matrix = glm::translate(glm::mat4(1.0f), parent->GetPosition()) *
							glm::rotate(glm::mat4(1.0f), parent->GetRotation().z, glm::vec3(0.0f, 0.0f, 1.0f)) *
							glm::rotate(glm::mat4(1.0f), parent->GetRotation().y, glm::vec3(1.0f, 0.0f, 0.0f)) *
							glm::rotate(glm::mat4(1.0f), parent->GetRotation().x, glm::vec3(0.0f, 1.0f, 0.0f)) *
							glm::scale(glm::mat4(1.0f), parent->GetScale());
						shader->SetShaderUniform("uModel", &matrix);
						reinterpret_cast<ModelRenderer<Core::GraphicsAPIS::OpenGL>*>(renderable.get())->Render();
					}
					else {
						obsoletes.insert(std::make_pair(it.first, it2));
					}
				}
				};

			glDepthMask(GL_TRUE);
			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
			glCullFace(GL_BACK);
			glViewport(0, 0, mDimensions.x, mDimensions.y);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			{
				glm::mat4 view = cam.GetViewMatrix();
				glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 10000.0f);

				mGBuffer->Bind();
				mGBuffer->ClearBuffer();

				std::for_each(std::execution::unseq, mGroupedRenderables.begin(), mGroupedRenderables.end(), 
					[this, &obsoletes, &projection, &view, &f_grouprender](const std::pair<Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Renderable>>>& it) {

						it, it.first->Get()->Bind();
						it.first->Get()->SetShaderUniform("uTransform", &projection);
						it.first->Get()->SetShaderUniform("uView", &view);

					try {
						it.first->Get()->SetShaderUniform("uCameraPos", &cam.GetPositionRef());
						UploadLightDataToGPU(it.first);
						auto tex = Singleton<ResourceManager>::Instance().GetResource<Texture>("Content/Textures/Brick.png")->Get();
						tex->SetTextureType(Texture::TextureType::eDiffuse);
						tex->Bind();
						auto normals = Singleton<ResourceManager>::Instance().GetResource<Texture>("Content/Textures/BrickNormal.png")->Get();
						normals->SetTextureType(Texture::TextureType::eNormal);
						normals->Bind();
					}
					catch (...) {}
					f_grouprender(it, it.first->Get());

					});

				f_flushobosoletes();
			}

			glDepthMask(GL_FALSE);
			glDisable(GL_DEPTH_TEST);
		}

		// ------------------------------------------------------------------------
		/*! Lighting Pass
		*
		*   Using the buffers created on the geometry pass, we can 
		*		compute the lighting for each pixel
		*/ //----------------------------------------------------------------------
		void OpenGLPipeline::LightingPass() {
			glBindFramebuffer(GL_FRAMEBUFFER, NULL);
			glEnable(GL_BLEND);
			glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_ONE, GL_ONE);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mGBuffer->GetPositionTextureHandle());
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, mGBuffer->GetNormalTextureHandle());
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, mGBuffer->GetAlbedoTextureHandle());
			mGBuffer->BindLightingShader();
			glViewport(0, 0, mDimensions.x, mDimensions.y);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			UploadLightDataToGPU(mGBuffer->GetLightingShader());

			std::vector<glm::mat4> shadow_matrices;

			for (int i = 0; i < ::Graphics::Primitives::Light::sLightReg; i++) {
				mShadowBuffers[i].BindTexture(3 + i);
				auto up = glm::normalize(glm::cross(glm::cross(-::Graphics::Primitives::Light::sLightData[i].mPosition, glm::vec3(0, 1, 0)), -::Graphics::Primitives::Light::sLightData[i].mPosition));
				glm::mat4 lightProjection = glm::perspective(glm::radians(120.f), 1.33f, 2.f, 2000.f);
				glm::mat4 lightView = glm::lookAt(::Graphics::Primitives::Light::sLightData[i].mPosition, glm::vec3(0.0, -15, 50), glm::vec3(0, 1, 0));
				glm::mat4 shadow_matrix = lightProjection * lightView;
				shadow_matrices.push_back(shadow_matrix);
			}

			for (int i = 0; i < ::Graphics::Primitives::Light::sLightReg; i++) {
				mGBuffer->GetLightingShader()->Get()->SetShaderUniform("uShadowMatrix[" + std::to_string(i) + "]", shadow_matrices.data() + i);
			}

			mGBuffer->GetLightingShader()->Get()->SetShaderUniform("uCameraPos", &cam.GetPositionRef());
			RenderScreenQuad();
		}

		void OpenGLPipeline::RenderShadowMaps() {
			std::unordered_multimap<Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Renderable>>::const_iterator> obsoletes;

			auto f_flushobosoletes = [this, &obsoletes]() {
				std::for_each(std::execution::par, obsoletes.begin(), obsoletes.end(), [this, &obsoletes](std::pair<const Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Renderable>>::const_iterator> x) {
					std::vector<std::weak_ptr<Renderable>>& it = mGroupedRenderables.find(x.first)->second;
					it.erase(x.second);

					//If we don't have any other renderables, erase it
					if (!it.size()) mGroupedRenderables.erase(x.first);
					});
				};

			auto f_grouprender = [&obsoletes](const std::pair<Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Renderable>>>& it, ShaderProgram* shader) {
				//For each renderable in shader program
				for (std::vector<std::weak_ptr<Renderable>>::const_iterator it2 = it.second.begin(); it2 != it.second.end(); it2++) {
					//If it isn't expired
					if (auto renderable = it2->lock()) {
						const std::shared_ptr<Object> parent = renderable->GetParent().lock();
						glm::mat4 matrix = glm::translate(glm::mat4(1.0f), parent->GetPosition()) *
							glm::rotate(glm::mat4(1.0f), parent->GetRotation().z, glm::vec3(0.0f, 0.0f, 1.0f)) *
							glm::rotate(glm::mat4(1.0f), parent->GetRotation().y, glm::vec3(1.0f, 0.0f, 0.0f)) *
							glm::rotate(glm::mat4(1.0f), parent->GetRotation().x, glm::vec3(0.0f, 1.0f, 0.0f)) *
							glm::scale(glm::mat4(1.0f), parent->GetScale());
						shader->SetShaderUniform("uModel", &matrix);
						reinterpret_cast<ModelRenderer<Core::GraphicsAPIS::OpenGL>*>(renderable.get())->Render();
					}
					else {
						obsoletes.insert(std::make_pair(it.first, it2));
					}
				}
				};

			glCullFace(GL_FRONT);
			for (int i = 0; i < ::Graphics::Primitives::Light::sLightReg; i++) {
				mShadowBuffers[i].Bind();
				mShadowBuffers[i].Clear(true);

				auto up = glm::normalize(glm::cross(glm::cross(-::Graphics::Primitives::Light::sLightData[i].mPosition, glm::vec3(0, 1, 0)), -::Graphics::Primitives::Light::sLightData[i].mPosition));
				glm::mat4 lightProjection = glm::perspective(glm::radians(120.f), 1.33f, 2.f, 2000.f);
				glm::mat4 lightView = glm::lookAt(::Graphics::Primitives::Light::sLightData[i].mPosition, glm::vec3(0.0, -15, 50), glm::vec3(0, 1, 0));

				{
					const auto shadow = Singleton<ResourceManager>::Instance().GetResource<ShaderProgram>("Content/Shaders/Shadow.shader")->Get();
					shadow->Bind();
					shadow->SetShaderUniform("uProjection", &lightProjection);
					shadow->SetShaderUniform("uView", &lightView);

					std::for_each(std::execution::unseq, mGroupedRenderables.begin(), mGroupedRenderables.end(), [this, &shadow, &obsoletes, &f_grouprender](const std::pair<Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Renderable>>>& it) {
						f_grouprender(it, shadow);
						});

					f_flushobosoletes();
				}

				mShadowBuffers[i].Unbind();

			}
		}

		// ------------------------------------------------------------------------
		/*! Set Dimension
		*
		*   Sets the Viewport Size on OpenGL
		*/ //----------------------------------------------------------------------
		void OpenGLPipeline::SetDimensions(const glm::lowp_u16vec2& dim) {
			glViewport(0, 0, dim.x, dim.y);
			mDimensions = dim;
		}

		// ------------------------------------------------------------------------
		/*! Render Screen Quad
		*
		*   Renders a Quad on the screen that covers the whole viewport
		*/ //----------------------------------------------------------------------
		unsigned int quadVAO = 0;
		unsigned int quadVBO;
		void OpenGLPipeline::RenderScreenQuad()
		{
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

		// ------------------------------------------------------------------------
		/*! Update Uniform Buffers
		*
		*   Updates the Uniform Buffers on the GPU across Shaders
		*/ //----------------------------------------------------------------------
		void OpenGLPipeline::UpdateUniformBuffers() {
			glm::mat4 view = cam.GetViewMatrix();
			glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 10000.0f);
		
			glBindBuffer(GL_UNIFORM_BUFFER, mUniformBuffer);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &view);
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &projection);
			glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec3), &cam.GetPositionRef());
			glBindBuffer(GL_UNIFORM_BUFFER, NULL);
		}

		// ------------------------------------------------------------------------
		/*! Directional Light Pass
		*
		*   Updates the Uniform Buffers on the GPU across Shaders
		*/ //----------------------------------------------------------------------
		void OpenGLPipeline::DirectionalLightPass() {	
			mDirectionalLightShader->Get()->Bind();
			RenderScreenQuad();
		}
	}
}