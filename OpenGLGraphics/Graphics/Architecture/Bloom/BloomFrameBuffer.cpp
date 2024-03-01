//
//	BloomFrameBuffer.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 1/03/24
//	Copyright © 2024. All Rights reserved
//

#include "BloomFrameBuffer.h"
#include <glew.h>
#include <iostream>

namespace Graphics {
	namespace Architecture {
		namespace Bloom {
            // ------------------------------------------------------------------------
            /*! Default Constructor
            *
            *   Constructs the Bloom FrameBuffer, setting it as uninitialized
            */ //----------------------------------------------------------------------
            BloomFBO::BloomFBO() : 
                mInit(false) {}

            // ------------------------------------------------------------------------
            /*! Destructor
            *
            *   EMPTY FUNCTION
            */ //----------------------------------------------------------------------
            BloomFBO::~BloomFBO() {}

            // ------------------------------------------------------------------------
            /*! Init
            *
            *   Creates a Franebuffer with many sampling textures
            */ //----------------------------------------------------------------------
            bool BloomFBO::Init(unsigned int windowWidth, unsigned int windowHeight, unsigned int mipChainLength) {
                //Check that it wasn't initted beafore
                if(mInit)
                    throw BloomFBOException("Framebuffer was already 'Init'ed");

                glGenFramebuffers(1, &mFBO);
                glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

                glm::vec2 mipSize((float)windowWidth, (float)windowHeight);
                glm::ivec2 mipIntSize((int)windowWidth, (int)windowHeight);

                // Safety check
                if (windowWidth > (unsigned int)INT_MAX || windowHeight > (unsigned int)INT_MAX) {
                    throw BloomFBOException("Dimensions are too large");
                }

                //Create the target texture for every MIP
                for (unsigned int i = 0; i < mipChainLength; i++) {
                    bloomMip mip;

                    mipSize *= 0.5f;
                    mipIntSize /= 2;
                    mip.size = mipSize;
                    mip.intSize = mipIntSize;

                    glGenTextures(1, &mip.texture);
                    glBindTexture(GL_TEXTURE_2D, mip.texture);
                    // we are downscaling an HDR color buffer, so we need a float texture format
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_R11F_G11F_B10F,
                        (int)mipSize.x, (int)mipSize.y,
                        0, GL_RGB, GL_FLOAT, nullptr);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

                    mMipChain.emplace_back(mip);
                }

                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                    GL_TEXTURE_2D, mMipChain[0].texture, 0);

                // setup attachments
                unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
                glDrawBuffers(1, attachments);

                // check completion status
                int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
                if (status != GL_FRAMEBUFFER_COMPLETE) {
                    throw BloomFBOException("Framebuffer is incomplete");
                }

                glBindFramebuffer(GL_FRAMEBUFFER, NULL);
                mInit = true;
                return true;
            }

            // ------------------------------------------------------------------------
            /*! Destroy
            *
            *   Frees all of the MIP Target Textures
            */ //----------------------------------------------------------------------
            void BloomFBO::Destroy() {
                //Free all textures, one by one
                for (int i = 0; i < mMipChain.size(); i++) {
                    glDeleteTextures(1, &mMipChain[i].texture);
                    mMipChain[i].texture = 0;
                }
                glDeleteFramebuffers(1, &mFBO);
                mFBO = 0;
                mInit = false;
            }

            // ------------------------------------------------------------------------
            /*! Bind For Writing
            *
            *   Binds the framebuffer for drawing into it
            */ //----------------------------------------------------------------------
            void BloomFBO::BindForWriting() {
                glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
            }

            // ------------------------------------------------------------------------
            /*! Mip Chain
            *
            *   Returns the vector containing all the textures
            */ //----------------------------------------------------------------------
            const std::vector<BloomFBO::bloomMip>& BloomFBO::MipChain() const {
                return mMipChain;
            }
		}
	}
}