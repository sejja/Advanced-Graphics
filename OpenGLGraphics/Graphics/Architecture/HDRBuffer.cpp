//
//	GBuffer.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 12/02/24
//	Copyright � 2024 . All Rights reserved
//

#include "HDRBuffer.h"
#include "Core/Window/SDLWindow.h"
#include "Core/Singleton.h"

namespace Core {
	namespace Graphics {
		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*   Constructs a G-Buffer, with 3 Textures (Position, Normal, Albedo)
		*/ //----------------------------------------------------------------------
		HDRBuffer::HDRBuffer() {
			mLightingShader = Singleton<ResourceManager>::Instance().GetResource<ShaderProgram>("Content/Shaders/DeferredLighting.shader");
			auto dim = Singleton<SDLWindow>::Instance().GetDimensions();
			dim = { 1072, 780 };
			glGenFramebuffers(1, &mBuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, mBuffer);

			// - color + specular color buffer
			glGenTextures(1, &mTexture);
			glBindTexture(GL_TEXTURE_2D, mTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dim.x, dim.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, mTexture, 0);

			// - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
			unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0};
			glDrawBuffers(1, attachments);
			;
			// create and attach depth buffer (renderbuffer)
			unsigned int rboDepth;
			glGenRenderbuffers(1, &rboDepth);
			glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, dim.x, dim.y);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
			// finally check if framebuffer is complete
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				throw HDRBufferException("HDR-Buffer was unable to be created fully");
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*   Frees the resources
		*/ //----------------------------------------------------------------------
		HDRBuffer::~HDRBuffer() {
			glDeleteTextures(1, &mTexture);
			glDeleteBuffers(1, &mBuffer);
		}

		// ------------------------------------------------------------------------
		/*! Bind
		*
		*   Binds the G-Buffer for drawing
		*/ //----------------------------------------------------------------------
		void HDRBuffer::Bind() {
			glBindFramebuffer(GL_FRAMEBUFFER, mBuffer);
		}

		void HDRBuffer::BindLightingShader() {
			mLightingShader->Get()->Bind();
		}

		// ------------------------------------------------------------------------
		/*! Blit Depth Buffer
		*
		*   Copies the Depth Values, so we can Forward Render after Deferred
		*/ //----------------------------------------------------------------------
		void HDRBuffer::BlitDepthBuffer() {
			glBindFramebuffer(GL_READ_FRAMEBUFFER, mBuffer);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
			glBlitFramebuffer(
				0, 0, 1072, 780, 0, 0, 1072, 780, GL_DEPTH_BUFFER_BIT, GL_NEAREST
			);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		Asset<ShaderProgram> HDRBuffer::GetLightingShader() {
			return mLightingShader;
		}

		// ------------------------------------------------------------------------
		/*! Get Position Texture Handle
		*
		*   Returns the Handle for the position-stride texture
		*/ //----------------------------------------------------------------------
		GLuint HDRBuffer::GetTextureHandle() {
			return mTexture;
		}

		// ------------------------------------------------------------------------
		/*! Clear Buffer
		*
		*   G-Buffer clears the G-Buffer
		*/ //----------------------------------------------------------------------
		void HDRBuffer::ClearBuffer() {
			glClearColor(0.0, 0.0, 0.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
	}
}