//
//	SSAOBuffer.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 18/04/24
//	Copyright � 2024 . All Rights reserved
//

#include "SSAOBuffer.h"

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
			}
		}
	}
}