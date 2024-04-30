#include "CubemapFramebuffer.h"
#include <../glm/glm.hpp>
#include <../glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace Core
{
	namespace Graphics
	{
		CubemapFramebuffer::CubemapFramebuffer(const glm::u16vec2 dim) :
			mDimensions(dim) {
			glGenFramebuffers(1, &mBuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, mBuffer);

			// - position color buffer
			glGenTextures(1, &mPosition);
			glBindTexture(GL_TEXTURE_2D, mPosition);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, dim.x, dim.y, 0, GL_RGB, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mPosition, 0);

			// - normal color buffer
			glGenTextures(1, &mNormal);
			glBindTexture(GL_TEXTURE_2D, mNormal);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, dim.x, dim.y, 0, GL_RGB, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mNormal, 0);

			// - color + specular color buffer
			glGenTextures(1, &mAlbedoSpecular);
			glBindTexture(GL_TEXTURE_2D, mAlbedoSpecular);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, dim.x, dim.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, mAlbedoSpecular, 0);

			//Brightness color buffer
			glGenTextures(1, &mBrightness);
			glBindTexture(GL_TEXTURE_2D, mBrightness);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, dim.x, dim.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, mBrightness, 0);

			glGenTextures(1, &mDepthTexture);
			glBindTexture(GL_TEXTURE_2D, mDepthTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, dim.x, dim.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthTexture, 0);


			// - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
			GLenum attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(sizeof(attachments) / sizeof(GLenum), attachments);
			;
			// create and attach depth buffer (renderbuffer)

			// finally check if framebuffer is complete
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				printf("G-Buffer was unable to be created fully");
		}

		CubemapFramebuffer::~CubemapFramebuffer()
		{
			glDeleteTextures(1, &mPosition);
			glDeleteTextures(1, &mNormal);
			glDeleteTextures(1, &mAlbedoSpecular);
			glDeleteTextures(1, &mBrightness);
			glDeleteRenderbuffers(1, &mDepth);
			glDeleteBuffers(1, &mBuffer);
		}

		void CubemapFramebuffer::Bind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, mBuffer);
		}

		void CubemapFramebuffer::Unbind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void CubemapFramebuffer::RenderToCubeMap(std::function<void(glm::mat4 projection, glm::mat4 view)> renderFunc)
		{
		}
	}
}
