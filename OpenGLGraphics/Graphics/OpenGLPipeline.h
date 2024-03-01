//
//	OpenGLPipeline.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 14/01/23
//	Copyright � 2023. All Rights reserved
//

#ifndef _OPEN_GL_PIPELINE__H_
#define _OPEN_GL_PIPELINE__H_

#include "Core/Pipeline.h"
#include "Graphics/Primitives/ShaderProgram.h"
#include "Graphics/Primitives/Renderables.h"
#include "Tools/FrameBuffer.h"
#include "Graphics/Architecture/GBuffer.h"
#include "Graphics/Architecture/HDRBuffer.h"
#include "Graphics/Architecture/SamplingBuffer.h"
#include "Core/ParticleSystem/ParticleManager.h"

namespace Core {
	namespace Graphics {
		class OpenGLPipeline : public Pipeline {
		public:
			void Init() override;
			void PreRender() override;
			void Render() override;
			inline void PostRender() override;
			inline void Shutdown() override;
			virtual void SetDimensions(const glm::lowp_u16vec2& dim) override;
			inline void AddRenderable(const std::weak_ptr<Renderable>& renderer);
			void SetParticleManager(std::shared_ptr<Core::Particles::ParticleManager> particleManager);
			GBuffer* GetGBuffer();
			FrameBuffer* GetRenderFrameBuffer();
			GLuint GetRenderTexture();

		private:
			void UploadLightDataToGPU(const AssetReference<Core::Graphics::ShaderProgram>& shader);
			void GeometryPass();
			void RenderGUI();
			void FlushObsoletes(std::unordered_multimap<Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Renderable>>::const_iterator> obsoletes);
			void GroupRender(std::unordered_multimap<Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Renderable>>::const_iterator> obsoletes,
				const std::pair<Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Renderable>>>& it,
				ShaderProgram* shader);
			void LightingPass();
			void RenderShadowMaps();
			void RenderScreenQuad();
			void UpdateUniformBuffers();
			void RenderParticlesSystems();

			std::unordered_map<Asset<ShaderProgram>, std::vector<std::weak_ptr<Renderable>>> mGroupedRenderables;

			std::weak_ptr<Core::Particles::ParticleManager> particleManager;

			glm::lowp_u16vec2 mDimensions;
			std::vector<FrameBuffer> mShadowBuffers;
			std::unique_ptr<GBuffer> mGBuffer;
			std::unique_ptr<FrameBuffer> mFrameBuffer;
			std::unique_ptr<HDRBuffer> mHDRBuffer;
			std::unique_ptr<SamplingBuffer> mSamplingBuffer;
			GLuint mUniformBuffer;

			GLuint mScreenQuadVAO, mScreenQuadVBO;


			GLboolean AntiAliasing = false;
			Asset<ShaderProgram> RendererShader;
			float exposure = 1;
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
			mGroupedRenderables[(std::dynamic_pointer_cast<ModelRenderer<Core::GraphicsAPIS::OpenGL>>(renderer.lock()))->GetShaderProgram().lock()].push_back(renderer);
		}
	}
}
#endif