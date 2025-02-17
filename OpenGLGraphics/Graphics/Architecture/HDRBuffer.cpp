//
//	FrameBuffer.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 05/07/23
//	Copyright � 2023. All Rights reserved
//

#include "HDRBuffer.h"
#include "Graphics/Tools/OpenGLInfo.h"
#include "Core/Singleton.h"

namespace Core {
	namespace Graphics {
		// ------------------------------------------------------------------------
		/*! Constructor
		*
		*   Constructs a FrameBuffer of said dimensions
		*/ //----------------------------------------------------------------------
		HDRBuffer::HDRBuffer() noexcept :
			mHandle(NULL), mTexture(NULL), mDimensions() {}

		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*   Releases the FrameBuffer resources from the GPU
		*/ //----------------------------------------------------------------------
		HDRBuffer::~HDRBuffer() noexcept {
			Unbind();
			if (mTexture) glDeleteTextures(1, &mTexture);
			if (mHandle) glDeleteBuffers(1, &mHandle);
		}

		// ------------------------------------------------------------------------
		/*! Create
		*
		*   Creates a FrameBuffer allocation on the GPU
		*/ //----------------------------------------------------------------------		
		void HDRBuffer::Create() {
#ifdef _DEBUG
			if (mHandle) throw FrameBufferException("We already have a buffer assigned!");
#endif
			glGenFramebuffers(1, &mHandle);
		}

		// ------------------------------------------------------------------------
		/*! Create Render Texture
		*
		*   Allocates a new rendertarget associated with this render texture
		*/ //----------------------------------------------------------------------
		void HDRBuffer::CreateRenderTexture(glm::lowp_u16vec2 dimensions, bool depthonly, bool readable) {
#ifdef _DEBUG
			if (!mHandle) throw FrameBufferException("We don't have a framebuffer created yet. Did you forget to call Create()?");
#endif

			glGenTextures(1, &mTexture);
			glBindTexture(GL_TEXTURE_2D, mTexture);
			if (depthonly)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, dimensions.x, dimensions.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			else
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, dimensions.x, dimensions.y, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
			Bind();

			if (depthonly)
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mTexture, 0);
			else
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture, 0);

			//If it's not readable
			if (!readable) {
				glDrawBuffer(GL_NONE);
				glReadBuffer(GL_NONE);
			}

			if (!depthonly) {
				glGenRenderbuffers(1, &mDepth);
				glBindRenderbuffer(GL_RENDERBUFFER, mDepth);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, dimensions.x, dimensions.y); // use a single renderbuffer object for both a depth AND stencil buffer.
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mDepth); // now actually attach it
			}

			mDimensions = dimensions;

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) throw FrameBufferException("FrameBuffer is Incomplete");
		}

		// ------------------------------------------------------------------------
		/*! Bind
		*
		*   Sets this FrameBuffer as the current target for drawing operations
		*/ //----------------------------------------------------------------------
		void HDRBuffer::Bind() {
#ifdef _DEBUG
			if (!mHandle) throw FrameBufferException("We don't have a framebuffer created yet. Did you forget to call Create()?");
#endif

			auto& openglinfo = Singleton<OpenGLInfo>::Instance();

			//If this framebuffer is not currently binded
			if (openglinfo.mBindedBuffer != mHandle) {
				openglinfo.mBindedBuffer = mHandle;
				glBindFramebuffer(GL_FRAMEBUFFER, mHandle);

				if (mTexture)
					glViewport(0, 0, mDimensions.x, mDimensions.y);
			}
		}

		// ------------------------------------------------------------------------
		/*! Unbind
		*
		*   Unbinds this buffer
		*/ //----------------------------------------------------------------------
		void inline HDRBuffer::Unbind() noexcept {
			auto& openglinfo = Singleton<OpenGLInfo>::Instance();

			if (openglinfo.mBindedBuffer) {
				Singleton<OpenGLInfo>::Instance().mBindedBuffer = 0;
				glBindFramebuffer(GL_FRAMEBUFFER, NULL);
			}
		}

		// ------------------------------------------------------------------------
		/*! Bind Texture
		*
		*   Binds the framebuffer render texture
		*/ //----------------------------------------------------------------------
		void HDRBuffer::BindTexture(unsigned i) {
#ifdef _DEBUG
			if (!mTexture) throw FrameBufferException("We don't have a texture associated yer. Did you forget to call CreateRenderTexture()?");
#endif

			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, mTexture);
		}

		// ------------------------------------------------------------------------
		/*! Clear
		*
		*   Clears the FrameBuffer memory
		*/ //----------------------------------------------------------------------
		void HDRBuffer::Clear(bool depthOnly) {
			Bind();

			//Switch depending on wether we need a full clean or just the depth
			if (depthOnly) glClear(GL_DEPTH_BUFFER_BIT);
			else glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		GLuint HDRBuffer::GetTextureHandle() {
			return mTexture;
		}
		GLuint HDRBuffer::GetHandle()
		{
			return mHandle;
		}
	}
}