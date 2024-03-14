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
#include "Core/Editor/Editor.h"
#include "Graphics/Tools/OpenGLInfo.h"


using namespace Core::Graphics;
using namespace std;

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
			//glEnable(GL_FRAMEBUFFER_SRGB);
			glFrontFace(GL_CCW);
			glDisable(GL_BLEND);
			glDisable(GL_STENCIL_TEST);
			glClearColor(0.f, 0.f, 0.f, 0.f);
			mShadowBuffers.emplace_back();
			mShadowBuffers.emplace_back();
			mShadowBuffers.emplace_back();
			mShadowBuffers.emplace_back();
			mShadowBuffers[0].Create();
			mShadowBuffers[0].CreateRenderTexture({mDimensions.x * 4, mDimensions.y * 4}, true);
			mShadowBuffers[1].Create();
			mShadowBuffers[1].CreateRenderTexture({ mDimensions.x * 4, mDimensions.y * 4 }, true);
			mShadowBuffers[2].Create();
			mShadowBuffers[2].CreateRenderTexture({ mDimensions.x * 4, mDimensions.y * 4 }, true);
			mShadowBuffers[3].Create();
			mShadowBuffers[3].CreateRenderTexture({ mDimensions.x * 4, mDimensions.y * 4 }, true);
			mGBuffer = std::make_unique<GBuffer>();
			mDirectionalLightShader = Singleton<ResourceManager>::Instance().GetResource<ShaderProgram>("Content/Shaders/DirectionalLight.shader");


			mFrameBuffer = std::make_unique<FrameBuffer>();
			mFrameBuffer->Create();
			mFrameBuffer->CreateRenderTexture({ mDimensions.x, mDimensions.y });

			mHDRBuffer = std::make_unique<HDRBuffer>();
			mHDRBuffer->Create();
			mHDRBuffer->CreateRenderTexture({ mDimensions.x, mDimensions.y });
			RendererShader = Singleton<ResourceManager>::Instance().GetResource<ShaderProgram>("Content/Shaders/Renderer.shader");

			//-------------------------
			glEnable(GL_MULTISAMPLE);
			mSamplingBuffer = std::make_unique<SamplingBuffer>();
			mSamplingBuffer->Create();
			mSamplingBuffer->CreateRenderTexture({ mDimensions.x, mDimensions.y });
			//-------------------------

			float quadVertices[] = {
				// positions        // texture Coords
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			};
			// setup plane VAO
			glEnable(GL_PROGRAM_POINT_SIZE);
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

		GBuffer* OpenGLPipeline::GetGBuffer() {
			return mGBuffer.get();
		}

		GLuint  OpenGLPipeline::GetRenderTexture() {
			return mFrameBuffer->GetTextureHandle();
		}

		// ------------------------------------------------------------------------
		/*! PreRender
		*
		*   Clears the whole FrameBuffer
		*/ //----------------------------------------------------------------------
		void OpenGLPipeline::PreRender() {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}


		/* FlushObsoletes
		*
		*   
		*/ 
		void OpenGLPipeline::FlushObsoletes(unordered_multimap<Asset<ShaderProgram>, vector<weak_ptr<Renderable>>::const_iterator> obsoletes)
		{
			for_each(execution::par, obsoletes.begin(), obsoletes.end(), [this, &obsoletes](pair<const Asset<ShaderProgram>, vector<weak_ptr<Renderable>>::const_iterator> x) {

				vector<weak_ptr<Renderable>>& it = mGroupedRenderables.find(x.first)->second;
				it.erase(x.second);

				//If we don't have any other renderables, erase it
				if (!it.size()) mGroupedRenderables.erase(x.first);
				});
		}

		/* GroupRender
		*
		*   Prepare and render the GUI
		*/ 
		void OpenGLPipeline::GroupRender(unordered_multimap<Asset<ShaderProgram>, vector<weak_ptr<Renderable>>::const_iterator> obsoletes,
			const pair<Asset<ShaderProgram>, vector<weak_ptr<Renderable>>>& it,
			ShaderProgram* shader)
		{
			for (vector<weak_ptr<Renderable>>::const_iterator it2 = it.second.begin(); it2 != it.second.end(); it2++) {
				//If it isn't expired
				if (auto renderable = it2->lock()) {
					const shared_ptr<Object> parent = renderable->GetParent().lock();
					glm::mat4 matrix = glm::translate(glm::mat4(1.0f), parent->GetPosition()) *
						glm::rotate(glm::mat4(1.0f), parent->GetRotation().z, glm::vec3(0.0f, 0.0f, 1.0f)) *
						glm::rotate(glm::mat4(1.0f), parent->GetRotation().y, glm::vec3(1.0f, 0.0f, 0.0f)) *
						glm::rotate(glm::mat4(1.0f), parent->GetRotation().x, glm::vec3(0.0f, 1.0f, 0.0f)) *
						glm::scale(glm::mat4(1.0f), parent->GetScale());
					shader->SetShaderUniform("uModel", &matrix);
					reinterpret_cast<GLBModelRenderer<Core::GraphicsAPIS::OpenGL>*>(renderable.get())->Render();
				}
				else {
					obsoletes.insert(make_pair(it.first, it2));
				}
			}
			FlushObsoletes(obsoletes);
		}


		/* RenderGUI
		*
		*   Prepare and render the GUI
		*/
		void OpenGLPipeline::RenderGUI() {
			
			ImGui_ImplSDL2_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();


			// Create the docking environment
			ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground |
				ImGuiWindowFlags_MenuBar;

			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);

			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("InvisibleWindow", nullptr, windowFlags);
			ImGui::PopStyleVar(3);

			ImGuiID dockSpaceId = ImGui::GetID("InvisibleWindowDockSpace");

			ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
			ImGui::End();

			ImGui::GetStyle().FrameRounding = 4.0f;
			ImGui::GetStyle().GrabRounding = 4.0f;
			ImGui::GetStyle().WindowBorderSize = 0.0f;
			ImGui::GetStyle().WindowRounding = 7.0f;
			//ImGui::GetStyle().AntiAliasedLines = true;
			//ImGui::GetStyle().AntiAliasedFill = true;
			

			ImVec4* colors = ImGui::GetStyle().Colors;
			colors[ImGuiCol_::ImGuiCol_Text] = { 0.95f, 0.96f, 0.98f, 1.00f };
			colors[ImGuiCol_::ImGuiCol_TextDisabled] = { 0.36f, 0.42f, 0.47f, 1.00f };
			colors[ImGuiCol_::ImGuiCol_WindowBg] = { 0.02f, 0.045f, 0.055f, 1.00f };
			colors[ImGuiCol_::ImGuiCol_ChildBg] = { 0.15f, 0.18f, 0.22f, 0.20f };
			colors[ImGuiCol_::ImGuiCol_PopupBg] = { 0.08f, 0.08f, 0.08f, 0.94f };
			colors[ImGuiCol_::ImGuiCol_Border] = { 0.08f, 0.10f, 0.12f, 1.00f };
			colors[ImGuiCol_::ImGuiCol_BorderShadow] = { 0.00f, 0.00f, 0.00f, 0.00f };
			colors[ImGuiCol_::ImGuiCol_FrameBg] = { 0.20f, 0.25f, 0.29f, 0.50f };
			colors[ImGuiCol_::ImGuiCol_FrameBgHovered] = { 0.12f, 0.20f, 0.28f, 1.00f };
			colors[ImGuiCol_::ImGuiCol_FrameBgActive] = { 0.09f, 0.12f, 0.14f, 1.00f };
			colors[ImGuiCol_::ImGuiCol_TitleBg] = { 0.09f, 0.12f, 0.14f, 0.65f };
			colors[ImGuiCol_::ImGuiCol_TitleBgActive] = { 0.08f, 0.10f, 0.12f, 1.00f };
			colors[ImGuiCol_::ImGuiCol_TitleBgCollapsed] = { 0.00f, 0.00f, 0.00f, 0.51f };
			colors[ImGuiCol_::ImGuiCol_MenuBarBg] = { 0.15f, 0.18f, 0.22f, 0.00f };
			colors[ImGuiCol_::ImGuiCol_ScrollbarBg] = { 0.02f, 0.02f, 0.02f, 0.39f };
			colors[ImGuiCol_::ImGuiCol_ScrollbarGrab] = { 0.20f, 0.25f, 0.29f, 1.00f };
			colors[ImGuiCol_::ImGuiCol_ScrollbarGrabHovered] = { 0.18f, 0.22f, 0.25f, 1.00f };
			colors[ImGuiCol_::ImGuiCol_ScrollbarGrabActive] = { 0.09f, 0.21f, 0.31f, 1.00f };
			colors[ImGuiCol_::ImGuiCol_CheckMark] = { 0.28f, 0.56f, 1.00f, 1.00f };
			colors[ImGuiCol_::ImGuiCol_SliderGrab] = { 0.28f, 0.56f, 1.00f, 1.00f };
			colors[ImGuiCol_::ImGuiCol_SliderGrabActive] = { 0.37f, 0.61f, 1.00f, 1.00f };
			colors[ImGuiCol_::ImGuiCol_Button] = { 0.20f, 0.25f, 0.29f, 0.70f };
			colors[ImGuiCol_::ImGuiCol_ButtonHovered] = { 0.28f, 0.56f, 1.00f, 1.00f };
			colors[ImGuiCol_::ImGuiCol_ButtonActive] = { 0.06f, 0.53f, 0.98f, 1.00f };
			colors[ImGuiCol_::ImGuiCol_Header] = { 0.20f, 0.25f, 0.29f, 0.35f };
			colors[ImGuiCol_::ImGuiCol_HeaderHovered] = { 0.12f, 0.43f, 0.78f, 0.80f };
			colors[ImGuiCol_::ImGuiCol_HeaderActive] = { 0.26f, 0.59f, 0.98f, 1.00f };
			colors[ImGuiCol_::ImGuiCol_Separator] = { 0.20f, 0.25f, 0.29f, 1.00f };
			colors[ImGuiCol_::ImGuiCol_SeparatorHovered] = { 0.10f, 0.40f, 0.75f, 0.78f };
			colors[ImGuiCol_::ImGuiCol_SeparatorActive] = { 0.10f, 0.40f, 0.75f, 1.00f };
			colors[ImGuiCol_::ImGuiCol_ResizeGrip] = { 0.26f, 0.59f, 0.98f, 0.25f };
			colors[ImGuiCol_::ImGuiCol_ResizeGripHovered] = { 0.26f, 0.59f, 0.98f, 0.67f };
			colors[ImGuiCol_::ImGuiCol_ResizeGripActive] = { 0.26f, 0.59f, 0.98f, 0.95f };
			colors[ImGuiCol_::ImGuiCol_Tab] = { 0.11f, 0.15f, 0.17f, 1.00f };
			colors[ImGuiCol_::ImGuiCol_TabHovered] = { 0.26f, 0.59f, 0.98f, 0.80f };
			colors[ImGuiCol_::ImGuiCol_TabActive] = { 0.20f, 0.25f, 0.29f, 1.00f };
			colors[ImGuiCol_::ImGuiCol_TabUnfocused] = { 0.11f, 0.15f, 0.17f, 1.00f };
			colors[ImGuiCol_::ImGuiCol_TabUnfocusedActive] = { 0.11f, 0.15f, 0.17f, 1.00f };
			colors[ImGuiCol_::ImGuiCol_PlotLines] = { 0.61f, 0.61f, 0.61f, 1.00f };
			colors[ImGuiCol_::ImGuiCol_PlotLinesHovered] = { 1.00f, 0.43f, 0.35f, 1.00f };
			colors[ImGuiCol_::ImGuiCol_PlotHistogram] = { 0.90f, 0.70f, 0.00f, 1.00f };
			colors[ImGuiCol_::ImGuiCol_PlotHistogramHovered] = { 1.00f, 0.60f, 0.00f, 1.00f };
			colors[ImGuiCol_::ImGuiCol_TextSelectedBg] = { 0.26f, 0.59f, 0.98f, 0.35f };
			colors[ImGuiCol_::ImGuiCol_DragDropTarget] = { 1.00f, 1.00f, 0.00f, 0.90f };
			colors[ImGuiCol_::ImGuiCol_NavHighlight] = { 0.26f, 0.59f, 0.98f, 1.00f };
			colors[ImGuiCol_::ImGuiCol_NavWindowingHighlight] = { 1.00f, 1.00f, 1.00f, 0.70f };
			colors[ImGuiCol_::ImGuiCol_NavWindowingDimBg] = { 0.80f, 0.80f, 0.80f, 0.20f };
			colors[ImGuiCol_::ImGuiCol_ModalWindowDimBg] = { 0.80f, 0.80f, 0.80f, 0.35f };





			//Render editor
			Singleton<Editor>::Instance().Render(*this);

			ImGui::Begin("Exposure Window");
			ImGui::SliderFloat("Exposure", &exposure, 0, 5);
			ImGui::End();

			ImGui::Begin("Anti-Aliasing");

			if (ImGui::Button("Anti-Aliashing")) {
				AntiAliasing = !AntiAliasing;
				std::cout << (int)AntiAliasing << std::endl;
			}
			ImGui::End();
		}




		// ------------------------------------------------------------------------
		/*! Render
		*
		*   Renders every object in the scene
		*/ //----------------------------------------------------------------------
		void OpenGLPipeline::Render() {
			RenderGUI();
			
			RenderShadowMaps();
			Skybox::sCurrentSky->UploadSkyboxCubeMap();
			UpdateUniformBuffers();
			GeometryPass();

			//RenderParticlesSystems();

			//Bind and Clean
			if (AntiAliasing) {mSamplingBuffer->Bind();mSamplingBuffer->Clear();}
			else {mHDRBuffer->Bind();mHDRBuffer->Clear();}
			glEnable(GL_DEPTH_TEST);

			LightingPass();
			
			if (AntiAliasing) mGBuffer->BlitDepthBuffer(mSamplingBuffer->GetHandle());
			else mGBuffer->BlitDepthBuffer(mHDRBuffer->GetHandle());

			Skybox::sCurrentSky->Render(cam,*this);

			RenderParticlesSystems();

			if (AntiAliasing) 
			{
				glBindFramebuffer(GL_READ_FRAMEBUFFER, mSamplingBuffer->GetHandle());
				glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mHDRBuffer->GetHandle());
				glBlitFramebuffer(0, 0, mDimensions.x, mDimensions.y, 0, 0, mDimensions.x, mDimensions.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
			}

			mFrameBuffer->Bind();
			mFrameBuffer->Clear();

			mHDRBuffer->BindTexture();
			RendererShader->Get()->Bind();
			RendererShader->Get()->SetShaderUniform("exposure", exposure);
			RenderScreenQuad();

			mFrameBuffer->Unbind();

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

			glEnable(GL_DEPTH_TEST);
			glCullFace(GL_BACK);
			glViewport(0, 0, mDimensions.x, mDimensions.y);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.1f);

			{
				glm::mat4 view = cam.GetViewMatrix();
				glm::mat4 projection = glm::perspective(glm::radians(45.0f), GetAspectRatio(), 0.1f, 10000.0f);

				mGBuffer->Bind();
				mGBuffer->ClearBuffer();

				std::for_each(std::execution::unseq, mGroupedRenderables.begin(), mGroupedRenderables.end(), 
					[this, &obsoletes, &projection, &view](const std::pair<Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Renderable>>>& it) {

						it, it.first->Get()->Bind();

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
					GroupRender(obsoletes,it, it.first->Get());

					});
			}

				FlushObsoletes(obsoletes);
			}
		

		// ------------------------------------------------------------------------
		/*! Lighting Pass
		*
		*   Using the buffers created on the geometry pass, we can 
		*		compute the lighting for each pixel
		*/ //----------------------------------------------------------------------
		void OpenGLPipeline::LightingPass() {
			
			//glEnable(GL_BLEND);
			//glBlendEquation(GL_FUNC_ADD);
			//glBlendFunc(GL_ONE, GL_ONE);
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
				glm::mat4 lightProjection = glm::perspective(glm::radians(120.f), GetAspectRatio(), 2.f, 2000.f);
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

			glCullFace(GL_NONE);
			for (int i = 0; i < ::Graphics::Primitives::Light::sLightReg; i++) {
				mShadowBuffers[i].Bind();
				mShadowBuffers[i].Clear(true);

				auto up = glm::normalize(glm::cross(glm::cross(-::Graphics::Primitives::Light::sLightData[i].mPosition, glm::vec3(0, 1, 0)), -::Graphics::Primitives::Light::sLightData[i].mPosition));
				glm::mat4 lightProjection = glm::perspective(glm::radians(120.f), GetAspectRatio(), 2.f, 2000.f);
				glm::mat4 lightView = glm::lookAt(::Graphics::Primitives::Light::sLightData[i].mPosition, glm::vec3(0.0, -15, 50), glm::vec3(0, 1, 0));

				{
					const auto shadow = Singleton<ResourceManager>::Instance().GetResource<ShaderProgram>("Content/Shaders/Shadow.shader")->Get();
					shadow->Bind();
					shadow->SetShaderUniform("uProjection", &lightProjection);
					shadow->SetShaderUniform("uView", &lightView);

					std::for_each(std::execution::unseq, mGroupedRenderables.begin(), mGroupedRenderables.end(), [this, &shadow, &obsoletes](const std::pair<Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Renderable>>>& it) {
						GroupRender(obsoletes ,it, shadow);
						});

					FlushObsoletes(obsoletes);
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
			glm::mat4 projection = glm::perspective(glm::radians(45.0f), GetAspectRatio(), 0.1f, 10000.0f);
		
			glBindBuffer(GL_UNIFORM_BUFFER, mUniformBuffer);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &view);
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &projection);
			glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec3), &cam.GetPositionRef());
			//glBindBuffer(GL_UNIFORM_BUFFER, mFrameBuffer->GetHandle());
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

		void OpenGLPipeline::RenderParticlesSystems()
		{
			if (auto currentParticleManager = particleManager.lock()) 
			{
				currentParticleManager->Render(&cam);
			}
			else 
			{
				std::cout << "ERROR -> PARTICLEMANAGER WAS DESTROYED \n";
			}

		}

		void OpenGLPipeline::SetParticleManager(std::shared_ptr<Core::Particles::ParticleMangager> particleManager) {
			this->particleManager = particleManager;
		}
	}

}