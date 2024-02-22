//
//	GBuffer.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 12/02/24
//	Copyright � 2024 . All Rights reserved
//

#include "GBuffer.h"
#include "Core/Window/SDLWindow.h"
#include "Core/Singleton.h"

namespace Core {
	namespace Graphics {
		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*   Constructs a G-Buffer, with 3 Textures (Position, Normal, Albedo)
		*/ //----------------------------------------------------------------------
		GBuffer::GBuffer() {
			mLightingShader = Singleton<ResourceManager>::Instance().GetResource<ShaderProgram>("Content/Shaders/DeferredLighting.shader");
			auto dim = Singleton<SDLWindow>::Instance().GetDimensions();
			dim = { 1072, 780 };
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
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dim.x, dim.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, mAlbedoSpecular, 0);

			// - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
			unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
			glDrawBuffers(3, attachments);
;
			// create and attach depth buffer (renderbuffer)
			unsigned int rboDepth;
			glGenRenderbuffers(1, &rboDepth);
			glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, dim.x, dim.y);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
			// finally check if framebuffer is complete
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				throw GBufferException("G-Buffer was unable to be created fully");
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*   Frees the resources
		*/ //----------------------------------------------------------------------
		GBuffer::~GBuffer() {
			glDeleteTextures(1, &mPosition);
			glDeleteTextures(1, &mNormal);
			glDeleteTextures(1, &mAlbedoSpecular);
			glDeleteBuffers(1, &mBuffer);
		}

		// ------------------------------------------------------------------------
		/*! Bind
		*
		*   Binds the G-Buffer for drawing
		*/ //----------------------------------------------------------------------
		void GBuffer::Bind() {
			glBindFramebuffer(GL_FRAMEBUFFER, mBuffer);
		}

		void GBuffer::BindLightingShader() {
			mLightingShader->Get()->Bind();
		}

		// ------------------------------------------------------------------------
		/*! Blit Depth Buffer
		*
		*   Copies the Depth Values, so we can Forward Render after Deferred
		*/ //----------------------------------------------------------------------
		void GBuffer::BlitDepthBuffer(GLuint HDRBuffer) {
			glBindFramebuffer(GL_READ_FRAMEBUFFER, mBuffer);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, HDRBuffer); // write to default framebuffer
			//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			glBlitFramebuffer(
				0, 0, 1072, 780, 0, 0, 1072, 780, GL_DEPTH_BUFFER_BIT, GL_NEAREST
			);
			//glBindFramebuffer(GL_FRAMEBUFFER, HDRBuffer);
		}

		Asset<ShaderProgram> GBuffer::GetLightingShader() {
			return mLightingShader;
		}

		// ------------------------------------------------------------------------
		/*! Get Position Texture Handle
		*
		*   Returns the Handle for the position-stride texture
		*/ //----------------------------------------------------------------------
		GLuint GBuffer::GetPositionTextureHandle() {
			return mPosition;
		}

		// ------------------------------------------------------------------------
		/*! get Normal Texture Handle
		*
		*   Returns the Handle for the normal-stride texture
		*/ //----------------------------------------------------------------------
		GLuint GBuffer::GetNormalTextureHandle() {
			return mNormal;
		}

		// ------------------------------------------------------------------------
		/*! Geometry Pass
		*
		*   Returns the Handle for the albedo-stride texture
		*/ //----------------------------------------------------------------------
		GLuint GBuffer::GetAlbedoTextureHandle() {
			return mAlbedoSpecular;
		}

		// ------------------------------------------------------------------------
		/*! Clear Buffer
		*
		*   G-Buffer clears the G-Buffer
		*/ //----------------------------------------------------------------------
		void GBuffer::ClearBuffer() {
			glClearColor(0.0, 0.0, 0.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
	}
}