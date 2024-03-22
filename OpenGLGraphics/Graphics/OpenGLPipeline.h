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
#include "Debug/DebugShapes.h"
#include "Graphics/Architecture/Bloom/BloomRenderer.h"
#include "Graphics/Architecture/LightPass.h"

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

		private:
			void GeometryPass();

			void RenderShadowMaps();
			void UpdateUniformBuffers();

			void RenderGUI();
			void FlushObsoletes(std::unordered_multimap<Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Renderable>>::const_iterator> obsoletes);
			void GroupRender(std::unordered_multimap<Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Renderable>>::const_iterator> obsoletes,
				const std::pair<Asset<Core::Graphics::ShaderProgram>, std::vector<std::weak_ptr<Renderable>>>& it, 
				ShaderProgram* shader);
			void BloomPass();

			std::unordered_map<Asset<ShaderProgram>, std::vector<std::weak_ptr<Renderable>>> mGroupedRenderables;
			glm::vec2 mDimensions;
			std::unique_ptr<GBuffer> mGBuffer;
			std::unique_ptr<::Graphics::Architecture::LightPass> mLightPass;
			Asset<ShaderProgram> mDirectionalLightShader;
			GLuint mUniformBuffer;
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