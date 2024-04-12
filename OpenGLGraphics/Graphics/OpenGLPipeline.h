//
//	OpenGLPipeline.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 14/01/23
//	Copyright � 2023. All Rights reserved
//

#ifndef _OPEN_GL_PIPELINE__H_
#define _OPEN_GL_PIPELINE__H_

#include "Core/Graphics/Pipeline.h"
#include "Graphics/Primitives/ShaderProgram.h"
#include "Graphics/Primitives/Renderables.h"
#include "Tools/FrameBuffer.h"
#include "Graphics/Architecture/GBuffer.h"
#include "Graphics/Architecture/HDRBuffer.h"
#include "Graphics/Architecture/SamplingBuffer.h"
#include "Core/ParticleSystem/ParticleManager.h"
#include "Debug/DebugShapes.h"
#include "Graphics/Architecture/Bloom/BloomRenderer.h"
#include "Graphics/Architecture/LightPass.h"

using namespace Core::Graphics;


namespace Core {
	namespace Graphics {
		class OpenGLPipeline : public Pipeline {
		public:
			~OpenGLPipeline();
			void Init() override;
			void PreRender() override;
			void Render() override;
			inline void PostRender() override;
			inline void Shutdown() override;
			virtual void SetDimensions(const glm::lowp_u16vec2& dim) override;
			inline void AddRenderable(const std::weak_ptr<Renderable>& renderer);
			void SetParticleManager(std::shared_ptr<Core::Particles::ParticleMangager> particleManager);
			GBuffer* GetGBuffer();
			FrameBuffer* GetRenderFrameBuffer();
			GLuint GetRenderTexture();
			std::vector<FrameBuffer> GetShadowMappingBuffer() { return mShadowBuffers; };

			void setSceneFrameDimensions(const glm::lowp_u16vec2& dim) { sceneFrameDimensions = dim; }
			float GetAspectRatio() { return static_cast<float>(sceneFrameDimensions.x) / static_cast<float>(sceneFrameDimensions.y); }
			float& getExposure() { return exposure; }
			GLboolean& getAntiAliasing() { return AntiAliasing; }
			void setAntiAliasing(GLboolean aa) { AntiAliasing = aa; }
			GLuint GetBloomTexture() { return mBloomRenderer->BloomTexture(); }
		

			void updateRenderablesGroups(const Core::Assets::Asset<ShaderProgram>& curShader, const Core::Assets::Asset<ShaderProgram>& newShader, const std::shared_ptr<Renderable>& renderable);

		private:
			void GeometryPass();
			void RenderGUI();
			void FlushObsoletes(std::unordered_multimap<Core::Assets::Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Renderable>>::const_iterator> obsoletes);
			void GroupRender(std::unordered_multimap<Core::Assets::Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Renderable>>::const_iterator> obsoletes,
				const std::pair<Core::Assets::Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Renderable>>>& it,
				ShaderProgram* shader);
			void LightingPass();
			void RenderShadowMaps();
			void UpdateUniformBuffers();
			void RenderParticlesSystems();
			void DirectionalLightPass();
			void BloomPass(GLuint targetbuffer);

			std::unordered_map<Core::Assets::Asset<ShaderProgram>, std::vector<std::weak_ptr<Renderable>>> mGroupedRenderables;

			std::weak_ptr<Core::Particles::ParticleMangager> particleManager;

			glm::lowp_u16vec2 mDimensions;
			glm::lowp_u16vec2 sceneFrameDimensions;
			std::vector<FrameBuffer> mShadowBuffers;
			std::unique_ptr<GBuffer> mGBuffer;
			std::unique_ptr<::Graphics::Architecture::LightPass> mLightPass;
			Core::Assets::Asset<ShaderProgram> mDirectionalLightShader;
			std::unique_ptr<FrameBuffer> mFrameBuffer;
			std::unique_ptr<HDRBuffer> mHDRBuffer;
			std::unique_ptr<SamplingBuffer> mSamplingBuffer;
			GLuint mUniformBuffer;

			GLuint mScreenQuadVAO, mScreenQuadVBO;


			GLboolean AntiAliasing = false;
			Core::Assets::Asset<ShaderProgram> RendererShader;
			float exposure = 5;
			std::unique_ptr<debug_system> mDebug;
			std::unique_ptr<::Graphics::Architecture::Bloom::BloomRenderer> mBloomRenderer;
		};

		// ------------------------------------------------------------------------
		/*! Post Render
		*
		*   EMPTY FUNCTION
		*/ //----------------------------------------------------------------------
		void OpenGLPipeline::PostRender() {}

		// ------------------------------------------------------------------------
		/*! Shut Down
		*
		*   EMPTY FUNCTION
		*/ //----------------------------------------------------------------------
		void OpenGLPipeline::Shutdown() {}
		
		// ------------------------------------------------------------------------
		/*! Add Renderable
		*
		*   Adds a Renderable into the pipeline
		*/ //----------------------------------------------------------------------
		void OpenGLPipeline::AddRenderable(const std::weak_ptr<Renderable>& renderer) {
			mGroupedRenderables[(std::dynamic_pointer_cast<GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>>(renderer.lock()))->GetShaderProgram().lock()].push_back(renderer);
		}
	}
}
#endif