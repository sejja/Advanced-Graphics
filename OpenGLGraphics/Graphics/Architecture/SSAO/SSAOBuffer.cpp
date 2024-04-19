//
//	SSAOBuffer.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 18/04/24
//	Copyright � 2024 . All Rights reserved
//

#include "SSAOBuffer.h"
#include "Kernel.h"
#include "Graphics/Architecture/Utils/GLUtils.h"
#include "gtc/matrix_transform.hpp"

namespace Graphics {
	namespace Architecture {
		namespace SSAO {
			// ------------------------------------------------------------------------
			/*! Conversion Constructor
			*
			*  Builds a SSAO Buffer with given dimensions
			*/ //----------------------------------------------------------------------
			SSAOBuffer::SSAOBuffer(const glm::u16vec2& dim) {
				glGenFramebuffers(1, &mHandle);
				glGenFramebuffers(1, &mBlurBuffer);
				glBindFramebuffer(GL_FRAMEBUFFER, mHandle);

				glGenTextures(1, &mColorBuffer);
				glBindTexture(GL_TEXTURE_2D, mColorBuffer);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, dim.x, dim.y, 0, GL_RED, GL_FLOAT, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorBuffer, 0);

				glBindFramebuffer(GL_FRAMEBUFFER, mBlurBuffer);
				glGenTextures(1, &mBlurTexture);
				glBindTexture(GL_TEXTURE_2D, mBlurTexture);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, dim.x, dim.y, 0, GL_RED, GL_FLOAT, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mBlurTexture, 0);

				mKernel = Kernel::SSAOKernel();
				mShaderSSAO = Singleton<Core::Assets::ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/SSAO.shader");
				mSSAOBlur = Singleton<Core::Assets::ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/SSAOBlur.shader");
			}

			// ------------------------------------------------------------------------
			/*! Render Ambient Oclussion
			*
			*  Renders the Ambient Oclussion buffer with the nonrmal and position textures
			*/ //----------------------------------------------------------------------
			void SSAOBuffer::RenderAO(const GBuffer& gbuffer) {
				glBindFramebuffer(GL_FRAMEBUFFER, mHandle);
				glClear(GL_COLOR_BUFFER_BIT);
				mShaderSSAO->Get()->Bind();
				// Send kernel + rotation 
				for (unsigned int i = 0; i < 64; ++i)
					mShaderSSAO->Get()->SetShaderUniform("samples[" + std::to_string(i) + "]", &mKernel[i]);
				//mShaderSSAO->Get()->SetShaderUniform("projection", &projection);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gbuffer.GetPositionTextureHandle());
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, gbuffer.GetNormalTextureHandle());
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, mNoise.GetNoiseTexture());
				Utils::GLUtils::RenderScreenQuad();
				glBindFramebuffer(GL_FRAMEBUFFER, 0);

				glBindFramebuffer(GL_FRAMEBUFFER, mBlurBuffer);
				glClear(GL_COLOR_BUFFER_BIT);
				mSSAOBlur->Get()->Bind();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, mColorBuffer);
				Utils::GLUtils::RenderScreenQuad();
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
		}
	}
}