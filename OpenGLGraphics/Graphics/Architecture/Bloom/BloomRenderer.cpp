//
//	BloomRenderer.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 1/03/24
//	Copyright © 2024. All Rights reserved
//

#include "BloomRenderer.h"
#include <glew.h>

namespace Graphics {
	namespace Architecture {
		namespace Bloom {
            // ------------------------------------------------------------------------
            /*! Default Constructor
            *
            *   Constructs an unitialized Bloom Renderer
            */ // ---------------------------------------------------------------------
            BloomRenderer::BloomRenderer() : 
                mInit(false) {}

            // ------------------------------------------------------------------------
            /*! Destrcutor
            *
            *   EMPTY FUNCTION
            */ // ---------------------------------------------------------------------
            BloomRenderer::~BloomRenderer() {}

            // ------------------------------------------------------------------------
            /*! Destroy
            *
            *   Destroys the FrameBuffer object of the bloom renderer
            */ // ---------------------------------------------------------------------
            void BloomRenderer::Destroy() {
                mFBO.Destroy();
                mInit = false;
            }

            // ------------------------------------------------------------------------
            /*! Render Bloom Texture
            *
            *   Performs bloom onto the binded framebuffer
            */ // ---------------------------------------------------------------------
            void BloomRenderer::RenderBloomTexture(unsigned int srcTexture, float filterRadius, GLuint targetbuffer) {
                mFBO.BindForWriting();

                this->RenderDownsamples(srcTexture);
                this->RenderUpsamples(filterRadius);

                glBindFramebuffer(GL_FRAMEBUFFER, targetbuffer);
                // Restore viewport
                glViewport(0, 0, mSrcViewportSize.x, mSrcViewportSize.y);
            }

            // ------------------------------------------------------------------------
            /*! Bloom Texture
            *
            *   Returns the final bloom texture
            */ // ---------------------------------------------------------------------
            GLuint BloomRenderer::BloomTexture() {
                return mFBO.MipChain()[0].texture;
            }

            // ------------------------------------------------------------------------
            /*! Init
            *
            *   Initialized a Bloom Renderer that targets the whole window
            */ // ---------------------------------------------------------------------
            bool BloomRenderer::Init(unsigned int windowWidth, unsigned int windowHeight) {
                if (mInit)
                    throw BloomRendererException("Bloom Renderer was already 'Init'ed");
                mSrcViewportSize = glm::ivec2(windowWidth, windowHeight);
                mSrcViewportSizeFloat = glm::vec2((float)windowWidth, (float)windowHeight);

                // Framebuffer
                const unsigned int num_bloom_mips = 5; // Experiment with this value
                bool status = mFBO.Init(windowWidth, windowHeight, num_bloom_mips);

                //If FBO creation failed
                if (!status) 
                    throw BloomRendererException("Failed to initialize bloom FBO - cannot create bloom renderer!");

                // Shaders
                mDownsampleShader = Singleton<Core::Assets::ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/Downsampling.shader");
                mUpsampleShader = Singleton<Core::Assets::ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/Upsampling.shader");

                float quadVertices[] = {
                    // upper-left triangle
                    -1.0f, -1.0f, 0.0f, 0.0f, // position, texcoord
                    -1.0f,  1.0f, 0.0f, 1.0f,
                     1.0f,  1.0f, 1.0f, 1.0f,
                     // lower-right triangle
                     -1.0f, -1.0f, 0.0f, 0.0f,
                      1.0f,  1.0f, 1.0f, 1.0f,
                      1.0f, -1.0f, 1.0f, 0.0f
                };
                glGenVertexArrays(1, &mScreenQuadVAO);
                glGenBuffers(1, &mScreenQuadVBO);

                glBindVertexArray(mScreenQuadVAO);
                glBindBuffer(GL_ARRAY_BUFFER, mScreenQuadVBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

                // position attribute
                glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);

                // texture coordinate
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                    (void*)(2 * sizeof(float)));
                glEnableVertexAttribArray(1);
                glBindVertexArray(0);
                mInit = true;
                return true;
            }

            unsigned int quadVAO = 0;
            unsigned int quadVBO;
            void RenderScreenQuad()
            {
                if (quadVAO == 0)
                {
                    float quadVertices[] = {
                        // positions        // texture Coords
                        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                    };
                    // setup plane VAO
                    glGenVertexArrays(1, &quadVAO);
                    glGenBuffers(1, &quadVBO);
                    glBindVertexArray(quadVAO);
                    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
                    glEnableVertexAttribArray(0);
                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
                    glEnableVertexAttribArray(1);
                    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
                }
                glBindVertexArray(quadVAO);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                glBindVertexArray(0);
            }

            // ------------------------------------------------------------------------
            /*! Render Downsamples
            *
            *   Renders all the Downsamples, before applying bloom
            */ // ---------------------------------------------------------------------
            void BloomRenderer::RenderDownsamples(unsigned int srcTexture) {
                std::vector<BloomFBO::BloomMip>& mipChain = mFBO.MipChain();

                mDownsampleShader->Get()->Bind();
                auto id = glGetUniformLocation(mDownsampleShader->Get()->GetHandle(), "srcResolution");
                glUniform2fv(id, 1, reinterpret_cast<float*>(&mSrcViewportSizeFloat));

                // Bind srcTexture (HDR color buffer) as initial texture input
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, srcTexture);

                // Progressively downsample through the mip chain
                for (int i = 0; i < mipChain.size(); i++)
                {
                    BloomFBO::BloomMip& mip = mipChain[i];
                    glViewport(0, 0, mip.size.x, mip.size.y);
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                        GL_TEXTURE_2D, mip.texture, 0);

                    // Render screen-filled quad of resolution of current mip
                    RenderScreenQuad();

                    // Set current mip resolution as srcResolution for next iteration
                    mDownsampleShader->Get()->SetShaderUniform("srcResolution", &mip.size);
       
                    auto id = glGetUniformLocation(mDownsampleShader->Get()->GetHandle(), "srcResolution");
                    glUniform2fv(id, 1, reinterpret_cast<float*>(&mip.size));
                    // Set current mip as texture input for next iteration
                    glBindTexture(GL_TEXTURE_2D, mip.texture);
                }
            }

            // ------------------------------------------------------------------------
            /*! Render Upsamples
            *
            *   Renders back upsampling, applying the bloom effect
            */ // ---------------------------------------------------------------------
            void BloomRenderer::RenderUpsamples(float filterRadius) {
                const std::vector<BloomFBO::BloomMip>& mipChain = mFBO.MipChain();

                mUpsampleShader->Get()->Bind();
                mUpsampleShader->Get()->SetShaderUniform("filterRadius", &filterRadius);

                // Enable additive blending
                glEnable(GL_BLEND);
                glBlendFunc(GL_ONE, GL_ONE);
                glBlendEquation(GL_FUNC_ADD);

                for (int i = mipChain.size() - 1; i > 0; i--)
                {
                    const BloomFBO::BloomMip& mip = mipChain[i];
                    const BloomFBO::BloomMip& nextMip = mipChain[i - 1];

                    // Bind viewport and texture from where to read
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, mip.texture);

                    // Set framebuffer render target (we write to this texture)
                    glViewport(0, 0, nextMip.size.x, nextMip.size.y);
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                        GL_TEXTURE_2D, nextMip.texture, 0);

                    // Render screen-filled quad of resolution of current mip
                    RenderScreenQuad();
                }

                // Disable additive blending
                glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); // Restore if this was default
                glDisable(GL_BLEND);
            }
		}
	}
}