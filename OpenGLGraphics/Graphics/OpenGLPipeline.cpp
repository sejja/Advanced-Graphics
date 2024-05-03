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
#include "Core/Input/InputManager.h"
#include "Graphics/Primitives/Lights/Light.h"
#include "Core/Singleton.h"
#include "Graphics/Primitives/Texture.h"
#include "Graphics/Primitives/Skybox.h"
#include "Dependencies/ImGui/imgui.h"
#include "Dependencies/ImGui/imgui_impl_opengl3.h"
#include "Dependencies/ImGui/imgui_impl_sdl2.h"
#include "Core/Editor/Editor.h"
#include "Graphics/Tools/OpenGLInfo.h"
#include "Graphics/Architecture/Utils/GLUtils.h"
#include "Graphics/Primitives/Decal.h"
#include "Graphics/Architecture/CubemapFramebuffer.h"


using namespace Core::Graphics;
using namespace std;

namespace Core {
	namespace Graphics {
		static Primitives::Camera cam;
		static GLuint reflectionCubemap;
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
			//glEnable(GL_FRAMEBUFFER_SRGB);
			glFrontFace(GL_CCW);
			glDisable(GL_BLEND);
			glDisable(GL_STENCIL_TEST);
			glClearColor(0.f, 0.f, 0.f, 0.f);
			mGBuffer = std::make_unique<::Graphics::Architecture::GBuffer>(mDimensions);
			mDirectionalLightShader = Singleton<Core::Assets::ResourceManager>::Instance().GetResource<ShaderProgram>("Content/Shaders/DeferredDirectionalLighting.shader");


			mFrameBuffer = std::make_unique<FrameBuffer>();
			mFrameBuffer->Create();
			mFrameBuffer->CreateRenderTexture({ mDimensions.x, mDimensions.y });

			mHDRBuffer = std::make_unique<HDRBuffer>();
			mHDRBuffer->Create();
			mHDRBuffer->CreateRenderTexture({ mDimensions.x, mDimensions.y });
			RendererShader = Singleton<Core::Assets::ResourceManager>::Instance().GetResource<ShaderProgram>("Content/Shaders/Renderer.shader");

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
			glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::vec3) + sizeof(glm::vec2), NULL, GL_STATIC_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			glBindBufferRange(GL_UNIFORM_BUFFER, 0, mUniformBuffer, 0, 2 * sizeof(glm::mat4) + sizeof(glm::vec3));
			mBloomRenderer = std::make_unique<::Graphics::Architecture::Bloom::BloomRenderer>(mDimensions);
			mLightPass = std::make_unique<::Graphics::Architecture::LightPass>();
			Singleton<::Editor>::Instance().assetManager.init();
			::Graphics::Architecture::Utils::GLUtils::Init();

			// CubemapReflections
			cubeMapDirections[0] = glm::vec3(1.0f, 0.0f, 0.0f);  // +X
			cubeMapDirections[1] = glm::vec3(-1.0f, 0.0f, 0.0f); // -X
			cubeMapDirections[2] = glm::vec3(0.0f, -1.0f, 0.0f);  // +Y
			cubeMapDirections[3] = glm::vec3(0.0f, 1.0f, 0.0f); // -Y
			cubeMapDirections[4] = glm::vec3(0.0f, 0.0f, -1.0f);  // +Z
			cubeMapDirections[5] = glm::vec3(0.0f, 0.0f, 1.0f); // -Z
			mDebug = std::make_unique<::Graphics::Debug::DebugSystem>();
			mSSAOBuffer = std::make_unique<::Graphics::Architecture::SSAO::SSAOBuffer>(mDimensions);
		}

		::Graphics::Architecture::GBuffer* OpenGLPipeline::GetGBuffer() {
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

		// ------------------------------------------------------------------------
		/*! RenderGUI
		*
		*   
		*/ 
		void OpenGLPipeline::FlushObsoletes(unordered_multimap<Core::Assets::Asset<ShaderProgram>, vector<weak_ptr<Renderable>>::const_iterator> obsoletes)
		{
			for_each(execution::par, obsoletes.begin(), obsoletes.end(), [this, &obsoletes](pair<const Core::Assets::Asset<ShaderProgram>, vector<weak_ptr<Renderable>>::const_iterator> x) {

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
		void OpenGLPipeline::GroupRender(unordered_multimap<Core::Assets::Asset<ShaderProgram>, vector<weak_ptr<Renderable>>::const_iterator> obsoletes,
			const pair<Core::Assets::Asset<ShaderProgram>, vector<weak_ptr<Renderable>>>& it,
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
					reinterpret_cast<GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>*>(renderable.get())->Render();
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
			Singleton<::Editor>::Instance().Render(*this);

		}


		void OpenGLPipeline::RenderShadowMaps(glm::mat4 view) {
			std::unordered_multimap<Core::Assets::Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Renderable>>::const_iterator> obsoletes;

			mLightPass->RenderShadowMaps({1600, 900}, view, [&obsoletes, this](ShaderProgram* shader) {
				//Render all objects
				std::for_each(std::execution::unseq, mGroupedRenderables.begin(), mGroupedRenderables.end(),
				[this, &obsoletes, &shader](const std::pair<Core::Assets::Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Renderable>>>& it) {
						GroupRender(obsoletes, it, shader);
					});

			FlushObsoletes(obsoletes);
				});
		}


		// ------------------------------------------------------------------------
		/*! Render
		*
		*   Renders every object in the scene
		*/ //----------------------------------------------------------------------
		void OpenGLPipeline::Render() {
			RenderGUI();

			
			
			RenderShadowMaps(cam.GetViewMatrix());
			Skybox::sCurrentSky->UploadSkyboxCubeMap();
			if (firstTime) {
				RenderReflectionCubemap(glm::vec3(0.0f,0.0f,0.0f));
				firstTime = false;
			}
			
			glActiveTexture(GL_TEXTURE11);
			glBindTexture(GL_TEXTURE_CUBE_MAP, reflectionCubemap);
			//RenderReflectionCubemap(cam.GetPosition());
			UpdateUniformBuffers(cam.GetViewMatrix(),cam.GetProjectionMatrix()); // SI
			GeometryPass({1600,900},cam.GetViewMatrix(), cam.GetProjectionMatrix()); //si
			mSSAOBuffer->RenderAO(*mGBuffer);
			mGeometryDeform.DecalPass(*mGBuffer);

			auto x = Singleton<::Editor>::Instance().GetSelectedObj().GetSelectedComponent();
			
			if (RTTI::IsA<::Graphics::Primitives::Decal>(x.get())) {
				mDebug->DrawAABB(x.get()->GetParent().lock()->GetPosition(),
					x.get()->GetParent().lock()->GetScale(), glm::vec4(1, 0.6, 0.2, 1), cam);
			}
			
			//Bind and Clean
			if (AntiAliasing) {mSamplingBuffer->Bind();mSamplingBuffer->Clear();}
			else {mHDRBuffer->Bind();mHDRBuffer->Clear();}
			//glEnable(GL_DEPTH_TEST);

			//RenderParticlesSystems();

			BloomPass(mHDRBuffer->GetHandle());
			mLightPass->RenderLights({1600, 900}, *mGBuffer, *mSSAOBuffer);

			if (AntiAliasing) mGBuffer->BlitDepthBuffer(mSamplingBuffer->GetHandle());
			else mGBuffer->BlitDepthBuffer(mHDRBuffer->GetHandle()); //SI

			Skybox::sCurrentSky->Render(cam, *this); //SI

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
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, mBloomRenderer->BloomTexture());
			::Graphics::Architecture::Utils::GLUtils::RenderScreenQuad();

			mFrameBuffer->Unbind();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			
		}


		void OpenGLPipeline::updateRenderablesGroups(const Core::Assets::Asset<ShaderProgram>& curShader, const Core::Assets::Asset<ShaderProgram>& newShader, const std::shared_ptr<Renderable>& renderable)
		{
			auto curShaderIt = mGroupedRenderables.find(curShader);
			auto newShaderIt = mGroupedRenderables.find(newShader);

			// Si se encuentra el curShader en el mapa
			if (curShaderIt != mGroupedRenderables.end()) {
				// Busca el renderable que ya estaba en el cursher
				auto& renderablesVector = curShaderIt->second;
				auto renderableIt = std::find_if(renderablesVector.begin(), renderablesVector.end(), [&](const std::weak_ptr<Renderable>& weakRenderable) {
					return !weakRenderable.expired() && weakRenderable.lock() == renderable;
					});

				// Si se encuentra el renderable, lo traslada al vector del nuevo shader
				if (renderableIt != renderablesVector.end()) {
					if (newShaderIt == mGroupedRenderables.end()) {
						// Si newShader no existe lo mete
						mGroupedRenderables[newShader] = std::vector<std::weak_ptr<Renderable>>();
						newShaderIt = mGroupedRenderables.find(newShader);
					}

					// curShader a --> newShader
					newShaderIt->second.push_back(*renderableIt);
					renderablesVector.erase(renderableIt);
				}
			}
		}

		// ------------------------------------------------------------------------
		/*! Geometry Pass
		*
		*   Draws the geometry on the G-Buffer
		*/ //----------------------------------------------------------------------
		void OpenGLPipeline::GeometryPass(const glm::u16vec2 dim, glm::mat4 view, glm::mat4 projection) {
			glDepthMask(GL_TRUE);
			glDisable(GL_BLEND);
			//glEnable(GL_DEPTH_TEST);
			glCullFace(GL_BACK);
			glViewport(0, 0, dim.x, dim.y);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.1f);

			std::unordered_multimap<Core::Assets::Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Renderable>>::const_iterator> obsoletes;

			{

				mGBuffer->Bind();
				mGBuffer->ClearBuffer();

				std::for_each(std::execution::unseq, mGroupedRenderables.begin(), mGroupedRenderables.end(),
					[this, &obsoletes, &projection, &view](const std::pair<Core::Assets::Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Renderable>>>& it) {

						it, it.first->Get()->Bind();

						GroupRender(obsoletes, it, it.first->Get());
					});
			}

			glDepthMask(GL_FALSE);
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
		void OpenGLPipeline::UpdateUniformBuffers(glm::mat4 view, glm::mat4 projection) {
		
			glBindBuffer(GL_UNIFORM_BUFFER, mUniformBuffer);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &view);
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &projection);
			glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec3), &cam.GetPositionRef());
			glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::vec3), sizeof(glm::vec2), &mDimensions);
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
			::Graphics::Architecture::Utils::GLUtils::RenderScreenQuad();
		}

		void OpenGLPipeline::BloomPass(GLuint targetbuffer)
		{
			mBloomRenderer->RenderBloomTexture(mGBuffer->GetBrightnessTextureHandle(), 0.005f, targetbuffer);
		}

		void OpenGLPipeline::DecalPass() {
			glDepthFunc(GL_GREATER);
			glDepthMask(GL_FALSE);
			glCullFace(GL_FRONT);

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
		unsigned int quadVAO = 0;
		unsigned int quadVBO;
	

		void OpenGLPipeline::RenderReflectionCubemap(const glm::vec3& position)
		{
		    // Set up framebuffer and cubemap
		    GLuint reflectionFramebuffer;
		   GLuint reflectionDepth;
		   int reflectionSize = 512;
		    
		    glGenFramebuffers(1, &reflectionFramebuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, reflectionFramebuffer);
		    glGenTextures(1, &reflectionCubemap);
		    glBindTexture(GL_TEXTURE_CUBE_MAP, reflectionCubemap);

		
		    // Set cubemap parameters
		    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		
			

		    //Set up cubemap faces
		    for (GLuint i = 0; i < 6; ++i)
		    {
		        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, reflectionSize, reflectionSize, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, reflectionCubemap, 0);
			}

			// Set the list of draw buffers.
			GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(4, drawBuffers); // Specify the number of color attachments

			glGenRenderbuffers(1, &reflectionDepth);
			glBindRenderbuffer(GL_RENDERBUFFER, reflectionDepth);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, reflectionSize, reflectionSize); // use a single renderbuffer object for both a depth AND stencil buffer.
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, reflectionDepth); // now actually attach it
			
			glActiveTexture(GL_TEXTURE11);
			glBindTexture(GL_TEXTURE_CUBE_MAP, reflectionCubemap);
			
			// Check if framebuffer is complete
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			{
				std::cerr << "Framebuffer is not complete!" << glGetError() << std::endl;
			}

		    // Render to each cubemap face
		    for (GLuint i = 0; i < 6; ++i)
		    {

				glViewport(0, 0, reflectionSize, reflectionSize);
				//glBindFramebuffer(GL_FRAMEBUFFER, reflectionFramebuffer);
				
				//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glm::mat4 view;
		        // Set the camera to look in the direction of the cubemap face
				if(i == 2) {
					//view = glm::lookAt(position, position + cubeMapDirections[i], glm::vec3(1.0f, 0.0f, 0.0f));
					view = CorrectRollDirection(true, -90.01f, 0.0f, 90.0f, position);
				}
				else if (i == 3) {
					glm::mat4 viewtre = glm::lookAt(position, position + cubeMapDirections[i], glm::vec3(-1.0f, 0.0f, 0.0f));
					view = CorrectRollDirection(false, 90.01f, 0.0f, -90.0f, position);
					
				}
				else {
					view = glm::lookAt(position, position + cubeMapDirections[i], glm::vec3(0.0f, 1.0f, 0.0f));
				}
				glm::mat4 projectionMatrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10000.0f);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, reflectionCubemap, 0);
		        // Render scene with this view matrix
		        // Call your rendering functions here, e.g., GeometryPass(), Skybox::sCurrentSky->Render(cam, *this)
				UpdateUniformBuffers(view, projectionMatrix);
				//RenderShadowMaps(view);
				GeometryPass({ 512, 512 },view,projectionMatrix);
				glBindFramebuffer(GL_FRAMEBUFFER, reflectionFramebuffer);
				BloomPass(reflectionFramebuffer);
				
				
				mLightPass->RenderLights({ 1600, 900}, *mGBuffer, *mSSAOBuffer);
				mGBuffer->BlitDepthBuffer(reflectionFramebuffer);
				Skybox::sCurrentSky->RenderSpecific(view,projectionMatrix, *this);
		        // You may want to read back the rendered data from each face if needed
				
				
				
		    }
		    // Cleanup
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			//glDeleteFramebuffers(1, &reflectionFramebuffer);
		    //glDeleteTextures(1, &reflectionCubemap);
			glViewport(0, 0, mDimensions.x, mDimensions.y);
		}

		glm::mat4 OpenGLPipeline::CorrectRollDirection(bool y, float pitch, float yaw, float roll, const glm::vec3& cameraPosition) {
			glm::vec3 front;
			front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			front.y = sin(glm::radians(pitch));
			front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
			glm::vec3 Front = glm::normalize(front);
			glm::vec3 Right;
			if(!y){
				Right = glm::normalize(glm::cross(Front, glm::vec3(1.0f, 0.0f, 0.0f)));
			}
			else {
				Right = glm::normalize(glm::cross(Front, glm::vec3(-1.0f, 0.0f, 0.0f)));
			}
			glm::vec3 Up = glm::normalize(glm::cross(Right, Front));

			// Apply roll
			glm::mat4 rollMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(roll), Front);
			Right = glm::vec3(rollMatrix * glm::vec4(Right, 0.0f));
			Up = glm::vec3(rollMatrix * glm::vec4(Up, 0.0f));

			return std::move(glm::lookAt(cameraPosition, cameraPosition + Front, Up));
		}
	}

}