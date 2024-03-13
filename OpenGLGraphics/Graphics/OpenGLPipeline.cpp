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

using namespace Core::Graphics;

namespace Core {
	namespace Graphics {
		static Primitives::Camera cam;

		OpenGLPipeline::~OpenGLPipeline() {
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplSDL2_Shutdown();
			ImGui::DestroyContext();
		}

		// ------------------------------------------------------------------------
		/*! Init
		*
		*   Sets the Initial OpenGL Settings to Draw
		*/ //----------------------------------------------------------------------
		void OpenGLPipeline::Init() {
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glEnable(GL_CULL_FACE);
			glEnable(GL_FRAMEBUFFER_SRGB);
			glFrontFace(GL_CCW);
			glDisable(GL_BLEND);
			glDisable(GL_STENCIL_TEST);
			glClearColor(0.f, 0.f, 0.f, 0.f);
			mGBuffer = std::make_unique<GBuffer>();
			mDirectionalLightShader = Singleton<ResourceManager>::Instance().GetResource<ShaderProgram>("Content/Shaders/DirectionalLight.shader");
			mDebug = std::make_unique<debug_system>(&cam);

			glGenBuffers(1, &mUniformBuffer);

			glBindBuffer(GL_UNIFORM_BUFFER, mUniformBuffer);
			glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::vec3) + sizeof(glm::vec2), NULL, GL_STATIC_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			glBindBufferRange(GL_UNIFORM_BUFFER, 0, mUniformBuffer, 0, 2 * sizeof(glm::mat4) + sizeof(glm::vec3));
			mBloomRenderer = std::make_unique<::Graphics::Architecture::Bloom::BloomRenderer>();
			mBloomRenderer->Init(mDimensions.x, mDimensions.y);
			mLightPass = std::make_unique<::Graphics::Architecture::LightPass>();
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
		/*! RenderGUI
		*
		*   Prepare and render the GUI
		*/ //----------------------------------------------------------------------
		void OpenGLPipeline::RenderGUI() {
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
				ImGui::SameLine();
				ImGui::Image((ImTextureID)mBloomRenderer->BloomTexture(),
					ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
			}
			ImGui::End();

		}

		// ------------------------------------------------------------------------
		/*! RenderGUI
		*
		*   Prepare and render the GUI
		*/ //----------------------------------------------------------------------
		void OpenGLPipeline::FlushObsoletes(std::unordered_multimap<Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Renderable>>::const_iterator> obsoletes) 
		{
			std::for_each(std::execution::par, obsoletes.begin(), obsoletes.end(), [this, &obsoletes](std::pair<const Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Renderable>>::const_iterator> x) {
				std::vector<std::weak_ptr<Renderable>>& it = mGroupedRenderables.find(x.first)->second;
				it.erase(x.second);

				//If we don't have any other renderables, erase it
				if (!it.size()) mGroupedRenderables.erase(x.first);
				});
		}

		// ------------------------------------------------------------------------
		/*! GroupRender
		*
		*   Prepare and render the GUI
		*/ //----------------------------------------------------------------------
		void OpenGLPipeline::GroupRender(std::unordered_multimap<Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Renderable>>::const_iterator> obsoletes,
			const std::pair<Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Renderable>>>& it,
			ShaderProgram* shader) 
		{
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
					reinterpret_cast<GLBModelRenderer<Core::GraphicsAPIS::OpenGL>*>(renderable.get())->Render();
				}
				else {
					obsoletes.insert(std::make_pair(it.first, it2));
				}
			}
			FlushObsoletes(obsoletes);
		}

		void OpenGLPipeline::BloomPass() {
			mBloomRenderer->RenderBloomTexture(mGBuffer->GetBrightnessTextureHandle(), 0.005f);
		}

		// ------------------------------------------------------------------------
		/*! Render
		*
		*   Renders every object in the scene
		*/ //----------------------------------------------------------------------
		void OpenGLPipeline::Render() {
			RenderGUI();
			
			std::vector<glm::mat4> shadow_mtrx = RenderShadowMaps();
			Skybox::sCurrentSky->UploadSkyboxCubeMap();
			UpdateUniformBuffers();
			GeometryPass();
			BloomPass();
			mLightPass->RenderLights(*mGBuffer, *mBloomRenderer, shadow_mtrx);
			glEnable(GL_DEPTH_TEST);
			mGBuffer->BlitDepthBuffer();
			Skybox::sCurrentSky->Render(cam);
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
						reinterpret_cast<GLBModelRenderer<Core::GraphicsAPIS::OpenGL>*>(renderable.get())->Render();
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
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.1f);

			{
				glm::mat4 view = cam.GetViewMatrix();
				glm::mat4 projection = cam.GetProjectionMatrix();

				mGBuffer->Bind();
				mGBuffer->ClearBuffer();

				std::for_each(std::execution::unseq, mGroupedRenderables.begin(), mGroupedRenderables.end(), 
					[this, &obsoletes, &projection, &view](const std::pair<Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Renderable>>>& it) {

						it, it.first->Get()->Bind();

					try {
						it.first->Get()->SetShaderUniform("uCameraPos", &cam.GetPositionRef());
						auto tex = Singleton<ResourceManager>::Instance().GetResource<Texture>("Content/Textures/Brick.png")->Get();
						tex->SetTextureType(Texture::TextureType::eDiffuse);
						tex->Bind();
						auto normals = Singleton<ResourceManager>::Instance().GetResource<Texture>("Content/Textures/BrickNormal.png")->Get();
						normals->SetTextureType(Texture::TextureType::eNormal);
						normals->Bind();
					}
					catch (...) {}
					GroupRender(obsoletes,it, it.first->Get());
					});
			}

			glDepthMask(GL_FALSE);
			glDisable(GL_DEPTH_TEST);
		}

		std::vector<glm::mat4> OpenGLPipeline::RenderShadowMaps() {
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
						reinterpret_cast<GLBModelRenderer<Core::GraphicsAPIS::OpenGL>*>(renderable.get())->Render();
					}
					else {
						obsoletes.insert(std::make_pair(it.first, it2));
					}
				}
				};

			std::vector<glm::mat4> shadow_matrices;

			glViewport(0, 0, mDimensions.x * 4, mDimensions.y * 4);
			for (int i = 0; i < ::Graphics::Primitives::Light::sLightReg; i++) {
				::Graphics::Primitives::Light::sLightData[i].mShadowMap.Bind();
				::Graphics::Primitives::Light::sLightData[i].mShadowMap.Clear(true);

				auto up = glm::normalize(glm::cross(glm::cross(-::Graphics::Primitives::Light::sLightData[i].mPosition, glm::vec3(0, 1, 0)), -::Graphics::Primitives::Light::sLightData[i].mPosition));
				glm::mat4 lightProjection = glm::perspective(glm::radians(120.f), 1.33f, 2.f, 2000.f);
				glm::mat4 lightView = glm::lookAt(::Graphics::Primitives::Light::sLightData[i].mPosition, ::Graphics::Primitives::Light::sLightData[i].mDirection, glm::vec3(0, 1, 0));
				glm::mat4 shadow_matrix = lightProjection * lightView;
				shadow_matrices.push_back(shadow_matrix);

				{
					const auto shadow = Singleton<ResourceManager>::Instance().GetResource<ShaderProgram>("Content/Shaders/Shadow.shader")->Get();
					shadow->Bind();
					shadow->SetShaderUniform("uProjection", &lightProjection);
					shadow->SetShaderUniform("uView", &lightView);
					glCullFace(GL_NONE);
					std::for_each(std::execution::unseq, mGroupedRenderables.begin(), mGroupedRenderables.end(), [this, &shadow, &obsoletes](const std::pair<Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Renderable>>>& it) {
						GroupRender(obsoletes,it, shadow);
						});
				}

				::Graphics::Primitives::Light::sLightData[i].mShadowMap.Unbind();

			}

			return shadow_matrices;
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
		/*! Update Uniform Buffers
		*
		*   Updates the Uniform Buffers on the GPU across Shaders
		*/ //----------------------------------------------------------------------
		void OpenGLPipeline::UpdateUniformBuffers() {
			glm::mat4 view = cam.GetViewMatrix();
			glm::mat4 projection = cam.GetProjectionMatrix();
		
			glBindBuffer(GL_UNIFORM_BUFFER, mUniformBuffer);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &view);
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &projection);
			glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec3), &cam.GetPositionRef());
			glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::vec3), sizeof(glm::vec2), &mDimensions);
			glBindBuffer(GL_UNIFORM_BUFFER, NULL);
		}
	}
}