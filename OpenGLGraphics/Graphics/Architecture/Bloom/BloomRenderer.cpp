//
//	BloomRenderer.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 1/03/24
//	Copyright © 2024. All Rights reserved
//

#include "BloomRenderer.h"
#include "Graphics/Architecture/LightPass.h"

namespace Graphics {
	namespace Architecture {
		namespace Bloom {
            // ------------------------------------------------------------------------
            /*! Default Constructor
            *
            *   Constructs an unitialized Bloom Renderer
            */ // ---------------------------------------------------------------------
            BloomRenderer::BloomRenderer(const glm::u16vec2 size) :
                mTexture({1600, 900}), mSrcViewportSize(size) {

                auto& resmgr = Singleton<Core::Assets::ResourceManager>::Instance();
                // Shaders
                mDownsampleShader = resmgr.GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/Downsampling.shader");
                mUpsampleShader = resmgr.GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/Upsampling.shader");
             }

            // ------------------------------------------------------------------------
            /*! Render Bloom Texture
            *
            *   Performs bloom onto the binded framebuffer
            */ // ---------------------------------------------------------------------
            void BloomRenderer::RenderBloomTexture(const GLuint srcTexture, float filterRadius, const GLuint targetbuffer) const {
                mTexture.Bind();
                RenderDownsamples(srcTexture);
                RenderUpsamples(filterRadius);

                glBindFramebuffer(GL_FRAMEBUFFER, targetbuffer);
                // Restore viewport
                glViewport(0, 0, mSrcViewportSize.x, mSrcViewportSize.y);
                // Disable additive blending
                glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); // Restore if this was default
                glDisable(GL_BLEND);
            }

            // ------------------------------------------------------------------------
            /*! Render Downsamples
            *
            *   Renders all the Downsamples, before applying bloom
            */ // ---------------------------------------------------------------------
            void BloomRenderer::RenderDownsamples(const GLuint srcTexture) const {
                const BloomFramebuffer::MipChain& mipChain = mTexture.GetMipChain();
                Core::Graphics::ShaderProgram* shader = mDownsampleShader->Get();
                glm::ivec2 srcResolution = glm::ivec2(mSrcViewportSize.x, mSrcViewportSize.y);
                shader->Bind();
                shader->SetShaderUniform("srcResolution", &srcResolution);

                // Bind srcTexture (HDR color buffer) as initial texture input
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, srcTexture);

                // Progressively downsample through the mip chain
                for (std::size_t i = 0; i < BloomFramebuffer::cNumMips;) {
                    const BloomFramebuffer::BloomMip& mip = mipChain[i++];
                    const glm::u16vec2 mipSize = mip.GetSize();
                    glViewport(0, 0, mipSize.x, mipSize.y);
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                        GL_TEXTURE_2D, mip.GetTexture(), NULL);

                    // Render screen-filled quad of resolution of current mip
                    LightPass::RenderScreenQuad();

                    glm::ivec2 srcResolution = glm::ivec2(mipSize.x, mipSize.y);
                    // Set current mip resolution as srcResolution for next iteration
                    shader->SetShaderUniform("srcResolution", &srcResolution);
                    // Set current mip as texture input for next iteration
                    glBindTexture(GL_TEXTURE_2D, mip.GetTexture());
                }
            }

            // ------------------------------------------------------------------------
            /*! Render Upsamples
            *
            *   Renders back upsampling, applying the bloom effect
            */ // ---------------------------------------------------------------------
            void BloomRenderer::RenderUpsamples(const float filterRadius) const {
                const BloomFramebuffer::MipChain& mipChain = mTexture.GetMipChain();
                Core::Graphics::ShaderProgram* shader = mUpsampleShader->Get();

                shader->Bind();
                shader->SetShaderUniform("filterRadius", &filterRadius);

                // Enable additive blending
                glEnable(GL_BLEND);
                glBlendFunc(GL_ONE, GL_ONE);
                glBlendEquation(GL_FUNC_ADD);

                for (std::size_t i = BloomFramebuffer::cNumMips - 1; i > 0; i--) {
                    const BloomFramebuffer::BloomMip& mip = mipChain[i];
                    const BloomFramebuffer::BloomMip& nextMip = mipChain[i - 1];
                    glm::ivec2 mipSize = glm::ivec2(nextMip.GetSize());

                    // Bind viewport and texture from where to read
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, mip.GetTexture());

                    // Set framebuffer render target (we write to this texture)
                    glViewport(0, 0, mipSize.x, mipSize.y);
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                        GL_TEXTURE_2D, nextMip.GetTexture(), NULL);

                    // Render screen-filled quad of resolution of current mip
                    LightPass::RenderScreenQuad();
                }
            }
		}
	}
}