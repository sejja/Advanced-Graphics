#include <iostream>
#include "SamplingBuffer.h"


namespace Core 
{
	namespace Graphics 
	{
		SamplingBuffer::SamplingBuffer() : FrameBuffer()
		{

		}
		SamplingBuffer::~SamplingBuffer()
		{
			Unbind();
			if (mTexture) glDeleteTextures(1, &mTexture);
			if (mHandle) glDeleteBuffers(1, &mHandle);
		}

		void SamplingBuffer::CreateRenderTexture(glm::lowp_u16vec2 dimensions, bool depthonly, bool readable) {
#ifdef _DEBUG
			if (!mHandle) throw FrameBufferException("We don't have a framebuffer created yet. Did you forget to call Create()?");
#endif
			glGenTextures(1, &mTexture);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mTexture);

			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA, dimensions.x, dimensions.y, GL_TRUE);

			glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_BORDER_COLOR, borderColor);

			Bind();

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, mTexture, 0);

			//If it's not readable
			if (!readable) {
				glDrawBuffer(GL_NONE);
				glReadBuffer(GL_NONE);
			}

			glGenRenderbuffers(1, &mDepth);
			glBindRenderbuffer(GL_RENDERBUFFER, mDepth);
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4,GL_DEPTH24_STENCIL8, dimensions.x, dimensions.y); // use a single renderbuffer object for both a depth AND stencil buffer.
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mDepth); // now actually attach it

			mDimensions = dimensions;

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) throw FrameBufferException("FrameBuffer is Incomplete");
		}
	}
}
