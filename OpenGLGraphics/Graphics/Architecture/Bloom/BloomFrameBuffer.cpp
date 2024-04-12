//
//	BloomFrameBuffer.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 1/03/24
//	Copyright © 2024. All Rights reserved
//

#include "BloomFrameBuffer.h"
 
namespace Graphics {
	namespace Architecture {
		namespace Bloom {
            // ------------------------------------------------------------------------
            /*! Default Constructor
            *
            *   Constructs the Bloom FrameBuffer, setting it as uninitialized
            */ //----------------------------------------------------------------------
            BloomFramebuffer::BloomFramebuffer(const glm::u16vec2 size) 
                :mMipChain{{BloomMip(size / unsigned short(1 << 1)), 
                    BloomMip(size / unsigned short(1 << 2)),
                    BloomMip(size / unsigned short(1 << 3)),
                    BloomMip(size / unsigned short(1 << 4)),
                    BloomMip(size / unsigned short(1 << 5))}} {
                glGenFramebuffers(1, &mTexture);
                glBindFramebuffer(GL_FRAMEBUFFER, mTexture);

                // Safety check
                if (size.x > UINT16_MAX || size.y > UINT16_MAX)
                    throw BloomFramebufferException("Dimensions are too large");

                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                    GL_TEXTURE_2D, mMipChain[0].GetTexture(), 0);

                // setup attachments
                const GLenum attachments[1] = { GL_COLOR_ATTACHMENT0 };
                glDrawBuffers(1, attachments);

                // check completion status
                if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                    throw BloomFramebufferException("Framebuffer is incomplete");

                glBindFramebuffer(GL_FRAMEBUFFER, NULL);
            }

            // ------------------------------------------------------------------------
            /*! Destructor
            *
            *   EMPTY FUNCTION
            */ //----------------------------------------------------------------------
            BloomFramebuffer::~BloomFramebuffer() {
                glDeleteFramebuffers(1, &mTexture);
            }

            // ------------------------------------------------------------------------
            /*! Bind For Writing
            *
            *   Binds the framebuffer for drawing into it
            */ //----------------------------------------------------------------------
            void BloomFramebuffer::Bind() const {
                glBindFramebuffer(GL_FRAMEBUFFER, mTexture);
            }

            // ------------------------------------------------------------------------
            /*! Mip Chain
            *
            *   Returns the vector containing all the textures
            */ //----------------------------------------------------------------------
            const BloomFramebuffer::MipChain& BloomFramebuffer::GetMipChain() const {
                return mMipChain;
            }

            // ------------------------------------------------------------------------
            /*! Conversion Constructor
            *
            *   Constructs a Bloom Mip with the given resolution
            */ //----------------------------------------------------------------------
            BloomFramebuffer::BloomMip::BloomMip(const glm::u16vec2 size) :
                mSize(size) {
                glGenTextures(1, &mTexture);
                glBindTexture(GL_TEXTURE_2D, mTexture);
                // we are downscaling an HDR color buffer, so we need a float texture format
                glTexImage2D(GL_TEXTURE_2D, 0, GL_R11F_G11F_B10F,
                    (int)size.x, (int)size.y,
                    0, GL_RGB, GL_FLOAT, nullptr);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            }

            // ------------------------------------------------------------------------
            /*! Destructor
            *
            *   Destroys the texture
            */ //----------------------------------------------------------------------
            BloomFramebuffer::BloomMip::~BloomMip() {
                glDeleteTextures(1, &mTexture);
            }
        }
	}
}