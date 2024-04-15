//
//	BloomRenderer.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 1/03/24
//	Copyright © 2024. All Rights reserved
//

#ifndef _BLOOM_RENDERER__H_
#define _BLOOM_RENDERER__H_

#include "BloomFrameBuffer.h"
#include "Graphics/Primitives/ShaderProgram.h"
#include "CommonDefines.h"

namespace Graphics{
	namespace Architecture {
		namespace Bloom {
            class BloomRenderer {
            #pragma region //Declarations
            public:
                CLASS_EXCEPTION(BloomRenderer)
            #pragma endregion

            #pragma region //Constructor & Destructor
                BloomRenderer(const glm::u16vec2 size);
            #pragma endregion

            #pragma region //Methods
                void RenderBloomTexture(const GLuint srcTexture, float filterRadius, const GLuint targetbuffer) const;
                DONTDISCARD GLuint inline BloomTexture() const;

            private:
                void RenderDownsamples(const GLuint srcTexture) const;
                void RenderUpsamples(const float filterRadius) const;
            #pragma endregion

            #pragma region //Members
                BloomFramebuffer mTexture;
                glm::u16vec2 mSrcViewportSize;
                Core::Assets::Asset<Core::Graphics::ShaderProgram> mDownsampleShader, mUpsampleShader;
            #pragma endregion
            };

            // ------------------------------------------------------------------------
            /*! Bloom Texture
            *
            *   Returns the final bloom texture
            */ // ---------------------------------------------------------------------
            GLuint BloomRenderer::BloomTexture() const {
                return mTexture.GetMipChain()[0].GetTexture();
            }
		}
	}
}

#endif