//
//	SSAOBuffer.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 18/04/24
//	Copyright � 2024 . All Rights reserved
//

#include "SSAOBuffer.h"
#include "Kernel.h"

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
				glBindFramebuffer(GL_FRAMEBUFFER, mHandle);

				glGenTextures(1, &mColorBuffer);
				glBindTexture(GL_TEXTURE_2D, mColorBuffer);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, dim.x, dim.y, 0, GL_RED, GL_FLOAT, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorBuffer, 0);

				glBindFramebuffer(GL_FRAMEBUFFER, mBlurBuffer);
				glGenTextures(1, &mBlurBuffer);
				glBindTexture(GL_TEXTURE_2D, mBlurBuffer);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, dim.x, dim.y, 0, GL_RED, GL_FLOAT, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mBlurBuffer, 0);
				mKernel = Kernel::SSAOKernel();
			}
			void SSAOBuffer::RenderAO(const GBuffer& gbuffer) {
				glBindFramebuffer(GL_FRAMEBUFFER, mHandle);
				glClear(GL_COLOR_BUFFER_BIT);
				/*shaderSSAO.use();
				// Send kernel + rotation 
				for (unsigned int i = 0; i < 64; ++i)
					shaderSSAO.setVec3("samples[" + std::to_string(i) + "]", ssaoKernel[i]);
				shaderSSAO.setMat4("projection", projection);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gPosition);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, gNormal);
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, noiseTexture);
				renderQuad();
				glBindFramebuffer(GL_FRAMEBUFFER, 0);*/
			}
		}
	}
}